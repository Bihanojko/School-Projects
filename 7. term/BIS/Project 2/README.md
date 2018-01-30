# Antispam

E-mail classifier, classifies input e-mails as spam or ham.

# Zadání
Vytvořte program pro detekci spamu v emilové komunikaci. Možno použít jazyky C, C++, Python. 

Po provedení příkazu "make" bude k dispozici spustitelný soubor "antispam". Program bude přijímat volitelné množství parametrů, kde každý parametr obsahuje cestu k souboru s emailem. Emailový soubor bude ve formátu .eml. Pro každý vstup soubor program vytiskne na standardní výstup: 
<název souboru> - <hodnocení> - důvod hodnocení \n 

možné hodnocení: 
OK - email není spam 
SPAM - email je spam 
FAIL - nepodařilo se ohodnotit (selhání programu apod.) 

Uvedení důvodu hodnocení není povinné, nebude mít vliv na hodnocení projektu. Může k němu však být přihlédnuto v případě nejasností/reklamací apod. 
Příklad spuštění: 
./antispam email.eml email2.eml email3.eml email4.eml 
email.eml - OK 
email2.eml - SPAM - contains word sex 
email3.eml - SPAM 
email4.eml - FAIL - failed to open email file 
