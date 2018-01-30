#!/usr/bin/env python3

from __future__ import print_function
import sys
import re
import argparse
import eml_parser
from enum import Enum
from bs4 import BeautifulSoup

from common_spam_words import common_spam_words_en, common_spam_words_cz, forbidden_words
from spam_signs import SpamSigns
from html_tag_list import html_tag_list


list_of_spam_signs = []


# enum of possible results
class ResultTypes(Enum):
    OK = 1
    SPAM = 2
    FAIL = 3



# print result of an e-mail
def print_result(result_value):
    if result_value == ResultTypes.OK:
        print(" - OK")
    elif result_value == ResultTypes.SPAM:
        print(" - SPAM - " + str(list_of_spam_signs))
    elif result_value == ResultTypes.FAIL:
        print(" - FAIL - failed to open e-mail file")



def is_body_empty(parsed_email):
    if 'body' not in parsed_email or len(parsed_email['body']) == 0 or 'content' not in parsed_email['body'][0]:
        return True

    return False



def get_sender_recipient_subject(parsed_email):
    sender = None
    recipient = None
    subject = None

    if 'header' in parsed_email:
        if 'from' in parsed_email['header']:
            sender = parsed_email['header']['from']

        if 'subject' in parsed_email['header']:
            subject = parsed_email['header']['subject']

        if 'to' in parsed_email['header']:
            recipient = parsed_email['header']['to']

    return (sender, recipient, subject)



def too_many_empty_lines(email_body):
    empty_lines_count = 0
    email_lines = email_body.split('\n')

    if len(email_lines) == 0 or email_body == '' or email_body.isspace():
        return 0

    for line in email_lines:
        if line.isspace() or line == '':
            empty_lines_count += 1

    if float(empty_lines_count / len(email_lines)) * 100.0 > 70:
        list_of_spam_signs.append(SpamSigns.MANY_EMPTY_LINES.value)
        return float(empty_lines_count / len(email_lines)) * 100.0

    return 0



def remove_html(email_content):
    soup = BeautifulSoup(email_content, "html5lib")
    for script in soup(["script", "style"]):
        script.extract()
    email_content = soup.get_text()

    words_in_email = re.findall(r"[\w']+", email_content)

    if len(words_in_email) == 0:
        return 1
    else:
        return len(words_in_email)



def get_bad_word_content_score(email_content, length_wo_html):
    global list_of_spam_signs
    bad_word_count = 0
    words_in_content = re.findall(r"[\w']+", email_content)
    email_content_lower = email_content.lower()
    uppercase_word_count = 0

    if len(words_in_content) == 0:
        return 0

    for bad_word in common_spam_words_en + common_spam_words_cz:
        occurences = re.findall(r'\s' + bad_word.replace(' ', '\s*') + r'\s', email_content_lower)
        # occurences = re.findall(r'(?<![a-zA-Z])' + bad_word.replace(' ', '\s*') + r'(?![a-zA-Z])', email_content)
        if occurences != []:
            bad_word_count += len(occurences)
            list_of_spam_signs.append("CONTAINS: " + bad_word)
            uppercase_word_count += email_content.count(bad_word.upper())

    for forbidden_word in forbidden_words:
        occurences = re.findall(r'\s' + forbidden_word.replace(' ', '\s*') + r'\s', email_content_lower)
        # occurences = re.findall(r'(?<![a-zA-Z])' + forbidden_word.replace(' ', '\s*') + r'(?![a-zA-Z])', email_content)
        if occurences != []:
            bad_word_count += len(occurences) * 15
            list_of_spam_signs.append("CONTAINS: " + forbidden_word)
            uppercase_word_count += email_content.count(forbidden_word.upper()) * 10      

    if uppercase_word_count != 0:
        list_of_spam_signs.append(SpamSigns.UPPERCASE_WORDS.value)

    return float((bad_word_count + uppercase_word_count / 2) / length_wo_html) * 100.0



def get_non_ascii_characters_score(email_content, length_wo_html):
    global list_of_spam_signs

    non_ascii_chars = 0
    for char in email_content:
        if ord(char) >= 128 and ord(char) not in [225, 205, 237, 269, 271, 233, 283, 328, 243, 345, 353, 357, 250, 367, 366, 253, 382, 193, 268, 270, 201, 282, 327, 211, 344, 352, 356, 218, 221, 381]:
            non_ascii_chars += 1

    if non_ascii_chars != 0:
        list_of_spam_signs.append(SpamSigns.NON_ASCII_CHARACTERS.value)

    return min(float(non_ascii_chars * 40.0 / length_wo_html), 5.0)



def get_many_single_chars_score(email_content, length_wo_html):
    global list_of_spam_signs
    words_in_content = re.findall(r"[\w']+", email_content)
    score = 0

    if len(words_in_content) == 0:
        return 0

    single_char_count = 0
    for word in words_in_content:
        if len(word) == 1:
            single_char_count += 1

    single_char_ratio = float(single_char_count / length_wo_html)
    if single_char_ratio > 0.5:
        score += single_char_ratio * 4
        list_of_spam_signs.append(SpamSigns.MANY_SINGLE_CHARS.value)

    return min(score, 3.0)



def get_html_content_score(email_content):
    global list_of_spam_signs
    score = 0.0
    for html_tag in html_tag_list:
        if email_content.count("<" + html_tag) != email_content.count("</" + html_tag):
            list_of_spam_signs.append(SpamSigns.OPEN_HTML_TAG.value)
            score += 0.5
        if "<bgsound" in email_content:
            list_of_spam_signs.append(SpamSigns.BGSOUND_TAG.value)
            score += 1.0

    soup = BeautifulSoup(email_content, "html5lib")
    if len(soup.find_all(b'font')) > 1:
        list_of_spam_signs.append(SpamSigns.MANY_FONTS.value)
        score += 10 * len(soup.find_all(b'font'))

    return score



def get_external_links_score(email_content):
    global list_of_spam_signs
    score = 0.0

    external_links = re.findall(r'http[s]?://[a-zA-Z0-9.:/\-~?=&_]*', email_content)
    fishy_external_links = re.findall(r'http[s]?\s+:\s+', email_content)
    score += len(fishy_external_links) * 2

    if external_links == []:
        return score

    if len(external_links) > 10:
        score += float(len(external_links) / 5.0) + 3.0
        list_of_spam_signs.append(SpamSigns.MANY_EXTERNAL_LINKS.value)

    for link in external_links:
        if re.search(r'(\.ru|goo\.gl|bit\.ly)', link) != None:
            score += 2.0
            list_of_spam_signs.append("URL CONTAINS: " + str(re.search(r'(\.ru|goo\.gl|bit\.ly)', link).group()))

    return min(score, 4.0)



def get_fishy_characters_score(email_content, length_wo_html):
    global list_of_spam_signs
    fishy_characters_score = 0

    for fishy_character in ['!', '?', '=']:
        if email_content.count(fishy_character) != 0:
            fishy_characters_score += float(email_content.count(fishy_character) / length_wo_html) * 20

    for fishy_character in ['$', '#', '%', '*', '€']:
        if email_content.count(fishy_character) != 0:
            fishy_characters_score += float(email_content.count(fishy_character) / length_wo_html) * 40

    if fishy_characters_score > 0.5:
        list_of_spam_signs.append(SpamSigns.MANY_FISHY_CHARACTERS.value)
        return min(fishy_characters_score, 2.0)

    return 0



def get_content_spam_score(email_content, content_type):
    global list_of_spam_signs
    score = 0.0
    value = 1.5 if content_type == 'subject' else 1.0
    length_wo_html = remove_html(email_content)

    score += get_bad_word_content_score(email_content, length_wo_html)
    email_content = email_content.lower()
    score += get_non_ascii_characters_score(email_content, length_wo_html)
    score += get_many_single_chars_score(email_content, length_wo_html)
    score += get_html_content_score(email_content)
    score += get_external_links_score(email_content)
    score += get_fishy_characters_score(email_content, length_wo_html)
    score += too_many_empty_lines(email_content)

    return score * value


def get_sender_score(sender):
    global list_of_spam_signs
    score = 0.0

    if sender.isspace():
        score += 3.0
    elif '@' not in sender:
        score += 3.0

    if score != 0.0:
        list_of_spam_signs.append(SpamSigns.FISHY_SENDER.value)

    return score



def get_message_id_score(email_content):
    global list_of_spam_signs

    if 'header' in email_content['header'] and 'message-id' in email_content['header']['header'] and len(email_content['header']['header']['message-id']) > 0:
        at_count = email_content['header']['header']['message-id'][0].count('@')
        if at_count != 1:
            list_of_spam_signs.append(SpamSigns.INVALID_MESSAGE_ID.value)
            return 10

        elif email_content['header']['header']['message-id'][0][1:email_content['header']['header']['message-id'][0].find('@')].isupper():
            list_of_spam_signs.append(SpamSigns.INVALID_MESSAGE_ID.value)
            return 10

    return 0



# check e-mail for signs of commonly used spam words
def check_email(parsed_email):
    global list_of_spam_signs
    score = 0.0

    (sender, recipient, subject) = get_sender_recipient_subject(parsed_email)
    if recipient == []:
        list_of_spam_signs.append(SpamSigns.RECIPIENT_EMPTY.value)
        score += 3

    if sender is None:
        list_of_spam_signs.append(SpamSigns.MISSING_SENDER.value)
        score += 3
    else:
        score += get_sender_score(sender)

    if subject != None:
        score += get_content_spam_score(subject, 'subject')

    score += get_message_id_score(parsed_email)

    if is_body_empty(parsed_email) or (len(parsed_email['body']) == 1 and parsed_email['body'][0]['content'] == ''):
        list_of_spam_signs.append(SpamSigns.BODY_EMPTY.value)
        score += 10

    else:
        for index in range(len(parsed_email['body'])):
            if 'content' in parsed_email['body'][index]:
                score += get_content_spam_score(parsed_email['body'][index]['content'], 'body')

    if 'attachment' in parsed_email and len(parsed_email['attachment']) > 0:
        list_of_spam_signs.append(SpamSigns.CONTAINS_ATTACHMENT.value)

        for single_attachment in parsed_email['attachment']:
            score += 2
            if 'filename' in single_attachment and '.' not in single_attachment['filename']:
                list_of_spam_signs.append(SpamSigns.FISHY_ATTACHMENT_FILENAME.value)
                score += 5

    # make values distinct
    list_of_spam_signs = list(set(list_of_spam_signs))

    if score < 38 or len(list_of_spam_signs) < 1:
        print_result(ResultTypes.OK)
    else:
        print_result(ResultTypes.SPAM)

    return



def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('email_filepaths', type=str, nargs='*',
                        help='paths to input e-mails')

    arguments = parser.parse_args()

    for single_email_path in arguments.email_filepaths:
        del list_of_spam_signs[:]
        print(single_email_path, end='')

        try:
            single_email = open(single_email_path, 'rb')
        except IOError:
            print_result(ResultTypes.FAIL)
            continue

        try:
            parsed_email = eml_parser.eml_parser.decode_email_b(single_email.read(), include_raw_body=True, include_attachment_data=True)
        except:
            list_of_spam_signs.append("error in decoding the e-mail")
            print_result(ResultTypes.SPAM)
            single_email.close()        
            continue

        single_email.close()

        check_email(parsed_email)



if __name__ == '__main__':
    main()
