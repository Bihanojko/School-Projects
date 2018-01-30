#!usr/bin/env python3

#CLS:xvales02


import sys
import io, codecs
import string, re
import copy
from lxml import etree
import lxml
import xml.etree.ElementTree as ET
from xml.dom.minidom import parse, parseString


# funkce na vypis napovedy
def print_help():
	print("CLS: C++ Classes\n"
	"Parametry:\n"
	"--help: vypise napovedu\n"
	"--input=file: vstupni textovy soubor file obsahujici popis trid jazyka C++\n"
	"--output=file: vystupni soubor ve formatu XML\n"
	"--pretty-xml=k: odsazeni o k mezer ve vystupnim souboru\n"
	"--details=class: vypsani udaju o clenech tridy se jmenem class\n"
	"--search=XPATH: vyhledani urcitych elementu pomoci XPATH\n")


# funkce na zpracovani argumentu a ulozeni jejich hodnot
# @param: vektor argumentu
def get_params(argv):
	params = {'input': sys.stdin, 'output': sys.stdout}

	for arg in argv:
		if arg == "--help" and len(argv) == 1:
			print_help()
			sys.exit(0)

		elif arg[:8] == '--input=' and params['input'] == sys.stdin and len(arg) > len('--input='):
			params['input'] = arg[8:]

		elif arg[:9] == '--output=' and params['output'] == sys.stdout and len(arg) > len('--output='):
			params['output'] = arg[9:]

		elif arg[:12] == '--pretty-xml' and 'pretty-xml' not in params:
			# --pretty-xml
			if arg[12:] == "":
				params['pretty-xml'] = 4
			else:
				# --pretty-xml=k
				if arg[13:].isdigit():
					params['pretty-xml'] = int(arg[13:])
				else:
					print("Chybne zadany parametr pretty-xml!", file=sys.stderr)
					sys.exit(1)

		elif arg[:9] == '--details' and 'details' not in params:
			# --details
			if arg[9:] == "":
				params['details'] = ""
			# --details=
			elif arg[9:] == "=":
				print("Chybne zadany parametr details!", file=sys.stderr)
				sys.exit(1)			
			# --details=classname
			else:
				params['details'] = arg[10:]

		elif arg[:9] == '--search=' and 'search' not in params:
			# --search=
			if arg[9:] == "":
				print("Chybne zadany parametr search!", file=sys.stderr)
				sys.exit(1)							
			# --search=XPath
			params['search'] = arg[9:]

		elif arg[:11] == '--conflicts' and 'conflicts' not in params:
			params['conflicts'] = True
		
		else:
			print("Spatne zadane argumenty!", file=sys.stderr)
			sys.exit(1)

	# implicitni odsazeni o k = 4
	if 'pretty-xml' not in params:
		params['pretty-xml'] = 4

	# parametr conflicts nezadan
	if 'conflicts' not in params:
		params['conflicts'] = False
	# parametr conflicts zadan, overeni, zda byl zadan i parametr details 
	else:
		if 'details' not in params:
			print("Spatne zadane argumenty!", file=sys.stderr)
			sys.exit(1)			

	return params


# funkce na zpracovani vstupniho souboru
# @param: data nactena ze vstupniho souboru
def parse_input(input_data):
	classname = []					# jmeno tridy
	attributes = []					# obsah definice tridy, deklarace clenu
	inheriting_class = []			# pole trid, ze kterych dana trida dedi
	in_data_parsed = []				# pole obsahujici vysledne udaje o vsech tridach
	state = "outside"				# pocatecni stav pro konecny automat
	token = ""						# jeden token v konecnem automatu

	# pro kazdy znak ve vstupnich datech
	for letter in input_data:

		# mimo telo definice tridy
		if state == "outside":
			if token == "class":
				token = ""
				state = "class_identif"
			elif letter.isspace() == False:
				token += letter

		# nacten token "class", nacita se jmeno tridy
		elif state == "class_identif":
			if letter == ":": 
				classname.append(token)
				token = ""
				state = "inheriting"
			elif letter == "{":
				classname.append(token)
				inheriting_class.append("")
				token = ""
				state = "in_class"
			elif letter == ";":
				classname.append(token)
				classname.append([""])
				classname.append([""])
				in_data_parsed.append(classname)
				classname = []
				attributes = []
				inheriting_class = []
				token = ""
				state = "outside"
			elif letter.isspace() == False:
				token += letter

		# ve jmenu tridy nacten znak ':', ocekava se prip. modifikator viditelnosti a jmeno dedene tridy
		elif state == "inheriting":
			if letter == "{":
				inheriting_class.append(token)
				token = ""
				state = "in_class"
			elif letter == ",":
				inheriting_class.append(token)
				token = ""
			elif letter.isspace() == False:
				token += letter

		# nacten znak '{', nacita se obsah tela tridy
		elif state == "in_class":
			if letter == "{":
				state = "func_decl"
			elif letter == "}":
				state = "endofclass"
			elif letter == ";":
				attributes.append(token)
				token = ""
			elif (letter.isspace() == True and token == "") == False:
				token += letter

		# v tele nacten znak '{', definice funkce, znaky zahazujeme az do '}' 
		elif state == "func_decl":
			if letter == "}":
				state = "in_class"

		# v tele tridy nacten znak '}', konec definice tridy
		elif state == "endofclass":
			if letter == ";":
				classname.append(inheriting_class)
				classname.append(attributes)
				in_data_parsed.append(classname)

				classname = []
				attributes = []
				inheriting_class = []
				state = "outside"

		else: state = "outside" 

	return in_data_parsed


# funkce, ktera provadi kontrolu vstupnich dat
# @param: pole zpracovanych udaju o tridach
def check_if_correct(in_data_parsed):
	# kontrola, zda trida nededi od nedeklarovane tridy
	for index, class_definition in enumerate(in_data_parsed):
		if class_definition[1] != [""]:
			for identif, inheriting_class in enumerate(class_definition[1]):
				correct = False
				inherit_name = []

				# oddeleni modifikatoru urovne pristupu
				if inheriting_class[:6] == "public":
					inherit_name.append(inheriting_class[6:])
					inherit_name.append("public")
				elif inheriting_class[:9] == "protected":
					inherit_name.append(inheriting_class[9:])
					inherit_name.append("protected")
				else:
					if inheriting_class[:7] == "private":
						inherit_name.append(inheriting_class[7:])
					else:
						inherit_name.append(inheriting_class)
					inherit_name.append("private")

				for i in range (0, index):
					if inherit_name[0] == in_data_parsed[i][0]:
						correct = True
				if correct == False:
					print("Chyba ve vstupnim souboru, dedeni od nedeklarovane tridy!", file=sys.stderr)
					sys.exit(4)

				in_data_parsed[index][1][identif] = inherit_name

	# pokud byla zadana pouze deklarace tridy (class A;), overi, zda byla i definovana
	delete = []
	for index, class_definition in enumerate(in_data_parsed):
		if class_definition[2] == [""]:
			correct = False
			for ind, class_def in enumerate(in_data_parsed):
				if ind != index and class_definition[0] == class_def[0] and class_def[2] != [""]:
					correct = True
					delete.append(index)
					break
			if correct == False:
				print("Chyba ve vstupnim souboru!", file=sys.stderr)
				sys.exit(4)

	if delete != []:
		delete.sort()
		delete = list(set(delete))
		for index in reversed(delete):
			del in_data_parsed[index]

	# kontrola vicenasobneho dedeni jedne tridy od jine
	for index, class_definition in enumerate(in_data_parsed):
		if class_definition[1] != [""]:
			inherited = []
			for identif, inheriting_class in enumerate(class_definition[1]):
				if inheriting_class[0] in inherited:
					print("Chyba ve vstupnim souboru, vicenasobne dedeni od jedne tridy!", file=sys.stderr)
					sys.exit(4)
				else:
					inherited.append(inheriting_class[0])

	return in_data_parsed



# funkce na zpracovani clenu trid
# @param: pole zpracovanych udaju o tridach
def parse_attributes(in_data_parsed):
	# pro kazdou definovanou tridu
	for index, class_definition in enumerate(in_data_parsed):
		# jestlize ma nejake cleny
		if class_definition[2] != []:
			rewrite = True
			visibility = "private"
			# pro kazdy clen tridy
			for attribute in class_definition[2]:
				attribute = attribute.strip()
				# nastaveni implicitnich hodnot atributu clenu
				scope = "instance"
				virtual = "no"
				kind = "concrete"
				typ = "method"
				name = ""
				type_of_att = ""
				parameters = ""
				using = ""
				param_name = []
				param_type = []

				# hledani modifikatoru viditelnosti
				pos = attribute.find("private:")
				if pos != -1:
					visibility = "private"
					attribute = attribute.replace("private:", "")
				pos = attribute.find("protected:")
				if pos != -1:
					visibility = "protected"
					attribute = attribute.replace("protected:", "")
				pos = attribute.find("public:")
				if pos != -1:
					visibility = "public"
					attribute = attribute.replace("public:", "")

				# hledani using
				pos = attribute.find("using ")
				if pos != -1:
					pos2 = attribute.find("::")
					using = attribute[pos + 6:pos2]
					attribute = attribute[:pos] + attribute[pos2 + 2:]

				# hledani static
				pos = attribute.find("static ")
				if pos != -1:
					scope = "static"
					attribute = attribute.replace("static ", "")

				# hledani virtual
				pos = attribute.find("virtual ")
				if pos != -1:
					virtual = "virtual"
					attribute = attribute.replace("virtual ", "")

				# hledani =0
				pos = re.search('=(\s)*0', attribute)
				if pos:
					if virtual != "virtual":
						print("Chyba ve vstupnim souboru!", file=sys.stderr)
						sys.exit(4)

					virtual = "pure virtual"
					kind = "abstract"
					attribute = re.sub('=(\s)*0', "", attribute)

				# hledani =
				pos = re.search('=', attribute)
				if pos:
					attribute = re.sub('(\s)*=(.)*', "", attribute)

				# hledani '(', pokud nenalezeno, jedna se o atribut
				pos = attribute.find("(")
				if pos == -1:
					typ = "attribute"

				# jinak se jedna o metodu
				else:
					# zpracovani argumentu teto metody
					parameters = attribute[pos + 1:]
					attribute = attribute[:pos]
					parameters = parameters.replace(")", "")
					parameters = parameters.strip()
					pos = parameters.find(",")
					if pos != -1:
						while pos != -1:
							single_param = parameters[:pos]
							parameters = parameters[pos + 1:]
							single_param = single_param.strip()
							pos = single_param.rfind(" ")
							if single_param[pos + 1] == "*" or single_param[pos + 1] == "&":
								param_name.append(single_param[pos + 2:])
								param_type.append(single_param[:pos + 2])
							else:
								param_name.append(single_param[pos + 1:])
								param_type.append(single_param[:pos])
							pos = parameters.find(",")

					pos = parameters.rfind(" ")
					if pos != -1 and pos < len(parameters) - 1:
						if parameters[pos + 1] == "*" or parameters[pos + 1] == "&":
							param_name.append(parameters[pos + 2:].strip())
							param_type.append(parameters[:pos + 2].strip())
						else:
							param_name.append(parameters[pos + 1:])
							param_type.append(parameters[:pos])
					else:
						param_name.append("")
						param_type.append(parameters)

				# zpracovani nazvu a typu clenu
				attribute = attribute.strip()
				pos = attribute.rfind(" ")
				if pos == -1:
					if attribute[0] == '~':
						type_of_att = "void"
					else:
						type_of_att = attribute
					name = attribute
				else:
					if pos < len(attribute) - 1:
						if attribute[pos + 1] == "*" or attribute[pos + 1] == "&":
							name = attribute[pos + 2:].strip()
							type_of_att = attribute[:pos + 2].strip()
						else:
							name = attribute[pos + 1:].strip()
							type_of_att = attribute[:pos].strip()
					else:
						name = attribute[pos + 1:].strip()
						type_of_att = attribute[:pos].strip()
						
				structure = []
				structure.append(name)
				structure.append(type_of_att)
				structure.append(typ)
				structure.append(visibility)
				structure.append(scope)
				structure.append(virtual)
				structure.append(kind)
				structure.append(using)
				structure.append(param_name)
				structure.append(param_type)
				structure.append("")
				structure.append("")

				if rewrite == True:
					rewrite = False
					in_data_parsed[index][2] = []
					in_data_parsed[index][2].append(structure)
				else:
					in_data_parsed[index][2].append(structure)

	return in_data_parsed


# kontrola spravosti datovych typu
# @param: pole zpracovanych udaju o tridach
def check_datatypes(in_data_parsed):
	correct = True
	data_types = ["int", "bool", "char", "float", "double", "void", "wchar_t", "signed", "unsigned", "short", "long", "*", "&"]

	input_types = ""
	for class_definition in in_data_parsed:
		data_types.append(class_definition[0])
		if class_definition[2] != [""]:
			for attribute in class_definition[2]:
				if attribute[7] == "":
					input_types += " "
					input_types += attribute[1]
					if attribute[9] != [""]:
						for single_type in attribute[9]:
							input_types += " "
							input_types += single_type

	input_types = input_types.split()
	for single_type in input_types:
		if single_type not in data_types:
			print("Chyba ve vstupnim souboru, nevalidni datovy typ!", file=sys.stderr)
			sys.exit(4)

	return


# funkce na zdedeni clenu
# @param: pole zpracovanych udaju o tridach
def inherit_attributes(in_data_parsed):
	correct = False

	# pro kazdou tridu
	for index, class_definition in enumerate(in_data_parsed):
		# pokud od nejake dedi
		if class_definition[1] != [""]:
			# pro kazdou tridu, ze ktere se dedi
			for inherit_class in class_definition[1]:

				# nalezeni zaznamu o tride, ze ktere se dedi
				for inherit_class_definition in in_data_parsed:
					if inherit_class_definition[0] == inherit_class[0]:
						correct = True

						# podle modifikatoru urovne pristupu, kopiruji se zaznamy o clenech
						if inherit_class[1] == "public":
							for attribute in inherit_class_definition[2]:

								if attribute[0] != attribute[1] and attribute[1] != "void":

									if attribute[3] == "public" or attribute[3] == "protected":
										attribute_change = copy.deepcopy(attribute)
										if attribute_change[10] == "":
											attribute_change[10] = inherit_class[0]
										attribute_change[11] = inherit_class[0]
										in_data_parsed[index][2].append(attribute_change)

									elif attribute[3] == "private" or attribute[3] == "NA":
										attribute_change = copy.deepcopy(attribute)
										if attribute_change[5] != "pure virtual":
											attribute_change[3] = "NA"
										if attribute_change[10] == "":
											attribute_change[10] = inherit_class[0]
										attribute_change[11] = inherit_class[0]
										in_data_parsed[index][2].append(attribute_change)

						elif inherit_class[1] == "protected":
							for attribute in inherit_class_definition[2]:

								if attribute[0] != attribute[1] and attribute[0] != "void":

									if attribute[3] == "public" or attribute[3] == "protected":
										attribute_change = copy.deepcopy(attribute)
										attribute_change[3] = "protected"
										if attribute_change[10] == "":
											attribute_change[10] = inherit_class[0]
										attribute_change[11] = inherit_class[0]
										in_data_parsed[index][2].append(attribute_change)

									elif attribute[3] == "private" or attribute[3] == "NA":
										attribute_change = copy.deepcopy(attribute)
										if attribute_change[5] != "pure virtual":
											attribute_change[3] = "NA"
										if attribute_change[10] == "":
											attribute_change[10] = inherit_class[0]
										attribute_change[11] = inherit_class[0]
										in_data_parsed[index][2].append(attribute_change)

						elif inherit_class[1] == "private":
							for attribute in inherit_class_definition[2]:

								if attribute[0] != attribute[1] and attribute[0] != "void":

									if attribute[3] == "public" or attribute[3] == "protected":
										attribute_change = copy.deepcopy(attribute)
										attribute_change[3] = "private"
										if attribute_change[10] == "":
											attribute_change[10] = inherit_class[0]
										attribute_change[11] = inherit_class[0]
										in_data_parsed[index][2].append(attribute_change)

									elif attribute[3] == "private" or attribute[3] == "NA":
										attribute_change = copy.deepcopy(attribute)
										if attribute_change[5] != "pure virtual":
											attribute_change[3] = "NA"
										if attribute_change[10] == "":
											attribute_change[10] = inherit_class[0]
										attribute_change[11] = inherit_class[0]
										in_data_parsed[index][2].append(attribute_change)
			
			class_definition[2] = check_overriding(class_definition[2])
			
			if correct == False:
				print("Chyba ve vstupnim souboru, neznama trida pro dedeni!", file=sys.stderr)
				sys.exit(4)

	return in_data_parsed


# funkce na kontrolu pretezovani a redeklarace
# @param: pole obsahujici udaje o clenech jedne tridy
def check_overriding(class_definition):
	delete = []
	for i, attribute in enumerate(class_definition):
		for j, other_att in enumerate(class_definition):
			if i != j and attribute[7] == "" and attribute[0] == other_att[0] and attribute[10] == "" and other_att[10] != "":
				if (attribute[2] == other_att[2] == "attribute") or \
				(attribute[2] == other_att[2] == "method" and (attribute[9] == other_att[9] or \
				(attribute[9] == ["void"] and other_att[9] == [""]) or (attribute[9] == [""] and other_att[9] == ["void"]))):
					delete.append(j)

	if delete != []:
		delete.sort()
		delete = list(set(delete))
		for index in reversed(delete):
			del class_definition[index]

	return class_definition


# funkce na pridani typu tridy - konkretni x abstraktni
# @param: pole zpracovanych udaju o tridach
def set_if_abstract(in_data_parsed):
	for index, class_definition in enumerate(reversed(in_data_parsed)):
		kind = "concrete"
		for i, attribute in enumerate(reversed(class_definition[2])):
			if attribute[5] == "pure virtual":
				kind = "abstract"

			if attribute[5] != "no" and attribute[4] == "static":
				print("Chyba ve vstupnim souboru, funkce staticka a zaroven virtualni!", file=sys.stderr)
				sys.exit(4)

		class_definition.append(kind)

	return in_data_parsed


# funkce na smazani udaju o konfliktnich clenech tridy classname ze trid, ktere od ni dedi
# @param: pole zpracovanych udaju o tridach
# @param: jmeno prave zpracovavane tridy
# @param: pole udaju o konfliktnim clenu
def delete_conflicts(in_data_parsed, classname, conflict):	
	for i, class_definition in enumerate(in_data_parsed):
		if class_definition[1] != [""]:
			for inherit_class in class_definition[1]:
				if inherit_class[0] == classname:
					delete = []
					for j, attribute in enumerate(class_definition[2]):
						if attribute[11] == classname:
							for index in range(11):
								if attribute[index] != conflict[index]:
									break
								if index == 10:
									delete.append(j)

					if delete != []:
						delete.sort()
						delete = list(set(delete))
						for index in reversed(delete):
							del in_data_parsed[i][2][index]

	return in_data_parsed


# funkce na kontrolu kofliktu, pripadne zaznamenani konfliktnich clenu
# @param: pole zpracovanych udaju o tridach
# @param: priznak urcujici, zda byl zadan prepinac conflicts
def check_conflicts(in_data_parsed, conflicts):
	# resi zabraneni konfliktu pouzitim using
	delete = []
	correct = False
	for index, class_definition in enumerate(in_data_parsed):
		for i, attribute in enumerate(class_definition[2]):
			if attribute[7] != "":
				correct = False
				for j, other_att in enumerate(class_definition[2]):
					if i != j and attribute[0] == other_att[0] and other_att[10] != "":
						if attribute[7] == other_att[11]:
							other_att[3] = attribute[3]
							correct = True
						else:
							delete.append(j)

				if correct == False:
					print("Chyba ve vstupnim souboru, neexistujici promenna za operatorem using!", file=sys.stderr)
					sys.exit(4)
				else:
					delete.append(i)

	if delete != []:
		delete.sort()
		delete = list(set(delete))
		for index in reversed(delete):
			del class_definition[2][index]

	# kontrola vzniku konfliktu
	found_conflicts = []
	indexes = []
	in_class = []
	correct = True
	for class_definition in in_data_parsed:
		single_conflict = []
		delete = []
		for i, attribute in enumerate(class_definition[2]):
			for j, other_att in enumerate(class_definition[2]):
				if i != j and attribute[0] == other_att[0]:
					if (attribute[2] != other_att[2]) or (attribute[2] == other_att[2] == "attribute"):	# jeden clen atribut a druhy metoda nebo oba atributy

						if attribute[10] == other_att[10] == "":
							print("Chyba ve vstupnim souboru, duplicitni deklarace clenu!", file=sys.stderr)
							sys.exit(4)

						if conflicts == True:
							if attribute[10] != "":
								in_class = []
								in_class.append(class_definition[0])
								delete.append(i)
								in_data_parsed = delete_conflicts(in_data_parsed, class_definition[0], attribute)
							if other_att[10] != "":
								in_class = []
								in_class.append(class_definition[0])
								delete.append(j)
								in_data_parsed = delete_conflicts(in_data_parsed, class_definition[0], other_att)

						else:
							correct = False
							break

					else:	# oba clenove jsou metody
						if len(attribute[9]) == len(other_att[9]):	# stejny pocet parametru
							for index, param in enumerate(attribute[9]):	# stejny typ parametru

								if other_att[9][index] != param:
									break

								if attribute[10] == other_att[10] == "":
									print("Chyba ve vstupnim souboru, duplicitni deklarace clenu!", file=sys.stderr)
									sys.exit(4)

								if index == len(attribute[9]) - 1:
									if conflicts == True:
										if attribute[10] != "":
											in_class = []
											in_class.append(class_definition[0])
											delete.append(i)
											in_data_parsed = delete_conflicts(in_data_parsed, class_definition[0], attribute)
										if other_att[10] != "":
											in_class = []
											in_class.append(class_definition[0])
											delete.append(j)
											in_data_parsed = delete_conflicts(in_data_parsed, class_definition[0], other_att)

									else:
										correct = False

					if in_class != []:
						indexes.append(in_class)

		delete.sort()
		delete = list(set(delete))
		single_conflict.append(class_definition[0])
		add = False
		for index in reversed(delete):
			add = True
			single_conflict.append(class_definition[2][index])
			del class_definition[2][index]

		if add == True:
			found_conflicts.append(single_conflict)

	# odstraneni konfliktu ze trid, ktere dedi od te, ve ktere konflikt vznikl
	if found_conflicts != []:
		for index, conflicts_in_class in enumerate(found_conflicts):
			delete = []
			for i, attribute in enumerate(conflicts_in_class[1:]):
				to_delete = True
				for class_definition in in_data_parsed:
					if class_definition[0] == conflicts_in_class[0]:
						for inherit_class in class_definition[1]:
							for other_class in in_data_parsed:
								if other_class[0] == inherit_class[0]:
									
									for other_att in other_class[2]:
										if attribute[0] == other_att[0] and attribute[1] == other_att[1] and attribute[2] == other_att[2]: 
											to_delete = False
						if to_delete == True:
							delete.append(i)

			delete.sort()
			delete = list(set(delete))
			for ind in reversed(delete):
				del found_conflicts[index][ind + 1]

	delete = []
	for index, one_conflict in enumerate(found_conflicts):
		if len(one_conflict) == 1:
			delete.append(index)

	delete.sort()
	delete = list(set(delete))
	for index in reversed(delete):
		del found_conflicts[index]

	if correct == False:
		print("Chyba ve vstupnim souboru, doslo ke konfliktu mezi dedenymi cleny!", file=sys.stderr)
		sys.exit(21)

	return (in_data_parsed, found_conflicts)


# funkce na vypis detailu o clenech s jednim typem viditelnosti (visibility)
# @param: pole udaju o tride, do ktere se aktualne dedi
# @param: retezec vystupnich dat
# @param: modifikator urovne pristupu k dedenym atributum
def print_one_visibility(class_definition, output_file, visibility):
	global current_indent
	printed = False
	first = True
	if (class_definition[2] != [""]):
		for attribute in class_definition[2]:

			# pokud se jedna o atribut 
			if attribute[2] == "attribute" and attribute[3] == visibility:
				if first == True:
					output_file = indent_line(output_file, "inc")
					output_file += "<%s>\n" % (visibility)
					output_file = indent_line(output_file, "inc")
					output_file += "<attributes>\n"
					printed = True
					first = False
				output_file = indent_line(output_file, "inc")
				output_file += "<attribute name=\"%s\" type=\"%s\" scope=\"%s\"" % (attribute[0], attribute[1], attribute[4])
				if attribute[10] != "":
					output_file += ">\n"
					output_file = indent_line(output_file, "inc")
					output_file += "<from name=\"%s\" />\n" % (attribute[10])
					output_file = indent_line(output_file, "dec")
					output_file += "</attribute>\n"
				else:
					output_file += " />\n"

		if printed == True:
			output_file = indent_line(output_file, "dec")
			output_file += "</attributes>\n"
			current_indent -= k

		first = True
		for attribute in class_definition[2]:
			# pokud se jedna o metodu
			if attribute[2] == "method" and attribute[3] == visibility:
				if printed == False:
					output_file = indent_line(output_file, "inc")
					output_file += "<%s>\n" % (visibility)
					printed = True
				if first == True:
					output_file = indent_line(output_file, "inc")
					output_file += "<methods>\n"
					first = False
				output_file = indent_line(output_file, "inc")
				output_file += "<method name=\"%s\" type=\"%s\" scope=\"%s\">\n" % (attribute[0], attribute[1], attribute[4])
				if attribute[10] != "":
					output_file = indent_line(output_file, "inc")
					output_file += "<from name=\"%s\" />\n" % (attribute[10])
					current_indent -= k
				if attribute[5] != "no":
					output_file = indent_line(output_file, "inc")
					if attribute[5] == "virtual":
						output_file += "<virtual pure=\"no\" />\n"
					else:
						output_file += "<virtual pure=\"yes\" />\n"
					current_indent -= k
				if attribute[8] != [""] and attribute[9] != ["void"]:
					output_file = indent_line(output_file, "inc")
					output_file += "<arguments>\n"
					current_indent += k
					for i, param in enumerate(attribute[8]):
						output_file = indent_line(output_file, "non")
						output_file += "<argument name=\"%s\" type=\"%s\" />\n" % (attribute[8][i], attribute[9][i])
					output_file = indent_line(output_file, "dec")
					output_file += "</arguments>\n"
					current_indent -= k
				else:
					output_file = indent_line(output_file, "inc")
					output_file += "<arguments></arguments>\n"
					current_indent -= k

				output_file = indent_line(output_file, "non")	
				output_file += "</method>\n"
				current_indent -= k

		if first == False:
			output_file = indent_line(output_file, "non")
			output_file += "</methods>\n"
			current_indent -= k
		if printed == True:
			output_file = indent_line(output_file, "non")
			output_file += "</%s>\n" % (visibility)
			current_indent -= k
	return output_file


# funkce na vypsani detailech o konfliktech ve tride classname
# @param: pole udaju o zaznamenanych konfliktech
# @param: retezec vystupnich dat
# @param: jmeno tridy, konflikty ve ktere chceme vypsat
def print_conflicts(conflicts, output_file, classname):
	global current_indent
	printed = []
	if conflicts != []:
		for one_class in conflicts:
			if one_class[0] == classname:
				output_file = indent_line(output_file, "inc")
				output_file += "<conflicts>\n"
				current_indent += k
				for single_conflict in one_class[1:]:
					if single_conflict[0] not in printed:
						printed_class = []
						printed.append(single_conflict[0])		
						output_file = indent_line(output_file, "non")
						output_file += "<member name=\"%s\">\n" % (single_conflict[0])
						current_indent += k
						for other_conflict in one_class[1:]:
							if other_conflict[0] == single_conflict[0] and other_conflict[10] not in printed_class:
								printed_class.append(other_conflict[10])
								output_file = indent_line(output_file, "non")
								output_file += "<class name=\"%s\">\n" % (other_conflict[10])
								output_file = indent_line(output_file, "inc")
							
								if other_conflict[3] == "NA":
									output_file += "<private>\n"
								else:
									output_file += "<%s>\n" % (other_conflict[3])

								output_file = indent_line(output_file, "inc")
								if other_conflict[2] == "attribute":
									output_file += "<attribute name=\"%s\" type=\"%s\" scope=\"%s\" />\n" % (other_conflict[0], other_conflict[1], other_conflict[4])
								else:
									output_file += "<method name=\"%s\" type=\"%s\" scope=\"%s\">\n" % (other_conflict[0], other_conflict[1], other_conflict[4])

									if other_conflict[5] != "no":
										output_file = indent_line(output_file, "inc")
										if other_conflict[5] == "virtual":
											output_file += "<virtual pure=\"no\" />\n"
										else:
											output_file += "<virtual pure=\"yes\" />\n"
										current_indent -= k
									if other_conflict[8] != [] and other_conflict[9][0] != "void":
										output_file = indent_line(output_file, "inc")
										output_file += "<arguments>\n"
										current_indent += k
										for i, param in enumerate(other_conflict[8]):
											output_file = indent_line(output_file, "non")
											output_file += "<argument name=\"%s\" type=\"%s\" />\n" % (other_conflict[8][i], other_conflict[9][i])
										output_file = indent_line(output_file, "dec")
										output_file += "</arguments>\n"
									else:
										output_file = indent_line(output_file, "inc")
										output_file += "<arguments></arguments>\n"
										current_indent -= k

									output_file = indent_line(output_file, "dec")
									output_file += "</method>\n"

								output_file = indent_line(output_file, "dec")
								if other_conflict[3] == "NA":
									output_file += "</private>\n"
								else:
									output_file += "</%s>\n" % (other_conflict[3])

								output_file = indent_line(output_file, "dec")
								output_file += "</class>\n"
				
						output_file = indent_line(output_file, "dec")
						output_file += "</member>\n"

				output_file = indent_line(output_file, "dec")
				output_file += "</conflicts>\n"
				current_indent -= k

	return output_file


# funkce na vypsani zakladnich prvku a volani dalsich funkci vypisu pro --conflicts
# @param: pole udaju o tride, jejiz detaily se maji vypsat
# @param: pole zpracovanych udaju o tridach
# @param: retezec vystupnich dat
# @param: pole udaju o zaznamenanych konfliktech
def print_class_details(class_definition, input_data, output_file, conflicts):
	global current_indent
	output_file = indent_line(output_file, "non")
	output_file += "<class name=\"%s\" kind=\"%s\">\n" % (class_definition[0], class_definition[3])
	if class_definition[1] != [""]:
		output_file = indent_line(output_file, "inc")
		output_file += "<inheritance>\n"
		current_indent += k
		for inheriting_class in class_definition[1]:
			output_file = indent_line(output_file, "non")
			output_file += "<from name=\"%s\" privacy=\"%s\" />\n" % (inheriting_class[0], inheriting_class[1])
		output_file = indent_line(output_file, "dec")
		output_file += "</inheritance>\n"
		current_indent -= k

	output_file = print_conflicts(conflicts, output_file, class_definition[0])
	output_file = print_one_visibility(class_definition, output_file, "public")
	output_file = print_one_visibility(class_definition, output_file, "protected")
	output_file = print_one_visibility(class_definition, output_file, "private")

	output_file = indent_line(output_file, "non")
	output_file += "</class>\n"
	return output_file


# funkce na vypis hlavicky pro --details a zpracovani hodnoty tohoto parametru
# @param: hodnota zadaneho parametru details
# @param: pole zpracovanych udaju o tridach
# @param: retezec vystupnich dat
# @param: pole zaznamenanych konfliktu
def print_details(required_class, input_data, output_file, conflicts):
	global current_indent
	output_file += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	if required_class == "":		# vypsani udaju o vsech tridach
		output_file += "<model>\n"
		current_indent += k
		for class_definition in input_data:
			output_file = print_class_details(class_definition, input_data, output_file, conflicts)
		output_file += "</model>\n"

	else:
		for index, class_definition in enumerate(input_data):
			if class_definition[0] == required_class:
				output_file = print_class_details(class_definition, input_data, output_file, conflicts)
	return output_file


# citac zanoreni
counter = 1
# zda bylo neco vypsano
printed = True


# funkce na vypsani dedicich trid
# @param: pole zpracovanych udaju o tridach
# @param: pole obsahujici udaje o tride, ktera se ma vypsat
# @param: retezec vystupnich dat
def print_inheriting_classes(in_data_parsed, class_definition, output_file):
	global current_indent
	global printed
	global counter

	for inheriting_class in in_data_parsed:
		if inheriting_class[1] != [""]:
			for inherit_data in inheriting_class[1]:
				if inherit_data[0] == class_definition[0]:

					if printed == False:
						output_file += ">\n"
					current_indent = (counter + 1) * k
					output_file = indent_line(output_file, "non")
					output_file += "<class name=\"%s\" kind=\"%s\"" % (inheriting_class[0], inheriting_class[3])

					counter += 1
					current_indent += k
					output_file = print_inheriting_classes(in_data_parsed, inheriting_class, output_file)
					current_indent -= k

					if printed == False:
						output_file += " />\n"
						current_indent -= k
						counter -= 1

					if counter > 1:
						counter -= 1
						output_file = indent_line(output_file, "non")
						output_file += "</class>\n"
					printed = True
		else:
			printed = False
	return output_file


# funkce na urceni, zda se jedna o korenovou tridu, od ktere ma zacit vypis
# @param: pole zpracovanych vstupnich dat o tridach
# @param: pole udaju o jedne tride, ktera se ma zpracovat
def check_if_root(in_data_parsed, class_definition):
	is_root = True
	is_inherited = False
	for other_class in in_data_parsed:
		if other_class[1] != [""]:
			for inherit_data in other_class[1]:
				if inherit_data[0] == class_definition[0]:
					is_inherited = True

	if class_definition[1] != [""]:
		is_root = False

	return (is_root and is_inherited)


# funkce ridici vypis dat bez prepinace details
# @param: pole zpracovanych vstupnich dat o tridach
# @param: retezec vystupnich dat
def print_normal_output(in_data_parsed, output_file):
	global current_indent
	output_file += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	output_file += "<model>\n"
	current_indent += k
	for class_definition in in_data_parsed:
		root = check_if_root(in_data_parsed, class_definition)
		if root == True:
			output_file = indent_line(output_file, "non")
			output_file += "<class name=\"%s\" kind=\"%s\"" % (class_definition[0], class_definition[3])
			current_indent += k
			output_file = print_inheriting_classes(in_data_parsed, class_definition, output_file)
			current_indent = k
			output_file = indent_line(output_file, "non")
			output_file += "</class>\n"
	output_file += "</model>\n"
	return output_file


# funkce, ktera zvysi, ci snizi aktualni hodnotu odsazeni a vlozi odsazeni
# @param: retezec vystupnich dat
# @param: operace, ktera ma byt provedena: "inc"/"dec" pro zvyseni/snizeni odsazeni
def indent_line(output_file, op):
	global current_indent
	if op == "inc":
		current_indent += k
	elif op == "dec":
		current_indent -= k

	for i in range (current_indent):
		output_file += " "
	return output_file


# funkce na vyhledani xpath_expr ve vyslednych datech
# @param: retezec vystupnich dat
# @param: hodnota zadaneho parametru search
def xpath_search(output_data, xpath_expr):
	global current_indent

	try:		
		root = ET.fromstring(output_data)
	except:
		return output_data

	root = etree.XML(output_data[39:])

	try:
		find_text = etree.XPath(xpath_expr)
	except:
		print("Neplatny XPath vyraz!", file=sys.stderr)
		sys.exit(1)

	text = find_text(root)

	output_data = output_data[:39]
	output_data += "<result>\n"
	current_indent += k

	try:
		result = etree.tostring(text[0], pretty_print=False)
		replacement = "\n"
		for i in range(k):
			replacement += " "
		result = str(result, 'utf-8-sig')
		result = re.sub('\n', replacement, result)
		result += '\n'
		output_data += replacement[1:]
		output_data += result
	except:
		text = sorted(text)
		for element in text:
			output_data = indent_line(output_data, "non")
			output_data += element
			output_data += "\n"

	output_data += "</result>\n"

	return output_data


current_indent = 0
k = 0


######## MAIN ########
def main(argv):
	params = get_params(argv)

	global k
	k = params['pretty-xml']

	if params['input'] != sys.stdin:
		try:
			input_file = codecs.open(params['input'], 'r', encoding='ascii')
		except IOError:
			print("Chyba pri otevirani vstupniho souboru!", file=sys.stderr)
			sys.exit(2)
	else:
		input_file = params['input']
		
	input_data = input_file.read()
	input_file.close()

	in_data_parsed = parse_input(input_data)

	in_data_parsed = check_if_correct(in_data_parsed)

	in_data_parsed = parse_attributes(in_data_parsed)

	check_datatypes(in_data_parsed)

	in_data_parsed = inherit_attributes(in_data_parsed)

	in_data_parsed = set_if_abstract(in_data_parsed)

	in_data_parsed, found_conflicts = check_conflicts(in_data_parsed, params['conflicts'])

	output_data = ""
	if 'details' in params:
		output_data = print_details(params['details'], in_data_parsed, output_data, found_conflicts)
	else:
		output_data = print_normal_output(in_data_parsed, output_data)


	if params['output'] != sys.stdout:
		try:
			output_file = codecs.open(params['output'], 'w', encoding='utf-8-sig')
		except IOError:
			print("Chyba pri otevirani vystupniho souboru!", file=sys.stderr)
			sys.exit(3)
	else:
		output_file = params['output']

	if 'search' in params:
		output_data = xpath_search(output_data, params['search'])

	output_file.write(output_data)
	output_file.close()

	sys.exit(0)



if __name__ == "__main__":
	main(sys.argv[1:])