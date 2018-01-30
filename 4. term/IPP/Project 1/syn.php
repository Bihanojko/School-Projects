<?php

#SYN:xvales02


// funkce na zpracovani a nacteni zadanych argumentu
// kontroluje pocet argumentu, kombinaci a duplicitu 
// parametry: vektor argumentu a pocet argumentu
// vraci asociativni pole params, ktere obsahuje hodnoty zadanych
// argumentu na indexech: input, format, output, br
function get_args ($argv, $argc)
{
	$params = array();
	foreach ($argv as $index) 
	{
		if ($argv[0] == $index)
			;
		else if (ereg("^--help$", $index) && $argc == 2)
		{
			print_help();
			exit(0);
		}
		else if (ereg("^(--format=)(.*)$", $index) && !isset($params["format"]))
		{
			$params["format"] = substr(strstr($index, '--format='), strlen("--format="));
		}
		else if (ereg("^(--input=)(.*)$", $index) && !isset($params["input"]))
		{
			$params["input"] = substr(strstr($index, '--input='), strlen("--input="));
		}
		else if (ereg("^(--output=)(.*)$", $index) && !isset($params["output"]))
		{
			$params["output"] = substr(strstr($index, '--output='), strlen("--output="));
		}
		else if (ereg("^--br$", $index) && !isset($params["br"]))
		{
			$params["br"] = TRUE;
		}
		else
		{
			print_error("Spatne zadane argumenty!\n");
			exit(1);
		}
	}
	return $params;
}


// funkce na nacteni obsahu vstupniho souboru
// overi, zda soubor existuje a lze jej otevrit pro cteni
// parametr: asociativni pole obsahujici zpracovane argumenty
// vraci obsah vstupniho souboru, nebo text zadany do stdin 
function get_input($params)
{
	if (isset($params["input"]))
	{
		if (file_exists($params["input"]) && is_readable($params["input"]))
			return (file_get_contents($params["input"]));
		else
		{
			print_error("Chyba pri otevirani vstupniho souboru!\n");
			exit(2);
		}
	}
	else
		return (file_get_contents("php://stdin"));
}


// funkce na nacteni obsahu vystupniho souboru
// overi, zda soubor existuje nebo lze vytvorit
// parametr: asociativni pole obsahujici zpracovane argumenty
// vraci nazev vystupniho souboru nebo stdout
function get_output($params)
{
	if (isset($params["output"]))
	{
		$output_file = $params["output"];
		if (($fw = fopen($output_file, 'w')) === FALSE)
		{
			print_error("Chyba pri otevirani, ci vytvareni vystupniho souboru!\n");
			exit(3);
		}
		else
			fclose($fw);
	}
	else
		$output_file = "php://stdout";

	return ($output_file);
}


// funkce na vypis chyboveho hlaseni na standardni chybovy vystup
// parametr: retezec s chybovym hlasenim
function print_error($error_message)
{
	file_put_contents("php://stderr", $error_message);
}


// funkce na vypis napovedy k programu
// ukonci program s navratovou hodnotou 0
function print_help()
{
	echo ("IPP - SYN: Zvyrazneni syntaxe\n\n".
	"Parametry:\n".
	"--help\tvypise napovedu\n".
	"--format=filename\turceni formatovaciho souboru\n".
	"--input=filename\turceni vstupniho souboru\n".
	"--output=filename\turceni vystupniho souboru\n".
	"--br\tprida element <br /> na konec kazdeho radku puvodniho vstupniho textu\n");
	exit (0);
}

// asociativni pole obsahujici zadane argumenty
$params = get_args($argv, $argc);
// pole obsahujici jednotlive regularni vyrazy z formatovaciho souboru
$reg_exps = array();
// pole obsahujici formatovaci prikazy z formatovaciho souboru
$tags = array();

// nacteni dat ze vstupniho souboru
$input_file = get_input($params);
// nastaveni vystupniho souboru
$output_file = get_output($params);


// nacteni, zpracovani a ulozeni dat z formatovaciho souboru
// nacteni souboru po radcich, preskoceni prazdnych radku, overeni, ze obsahuje \t
// ulozeni regularnich vyrazu do pole reg_exps
// ulozeni formatovacich prikazu do pole tags
// pokud nebyl formatovaci soubor zadan, pripadne se aplikuje znacka br
// a vstupni soubor se vypise do vystupniho
if (isset($params["format"]) && file_exists($params["format"]) && is_readable($params["format"]))
{
	$lines = file($params["format"]);

	foreach ($lines as $s_line => $content)
	{
		if (!(preg_match("/^[\s]*$/", $content)))
		{
			$pos = strpos($content, "\t");
			if ($pos !== FALSE)
			{
				$reg_exps[$s_line] = substr($content, 0, $pos);
				$tmp = substr($content, $pos);
				$tmp = preg_replace('/\s+/', '', $tmp);
				$tags[] = explode(",", $tmp);
			}
			else
			{
				print_error("Chyba ve formatovacim souboru!\n");
				exit(4);				
			}
		}
	}
}
else
{
	if (isset($params["br"]))
	$input_file = str_replace("\n", "<br />\n", $input_file);

	file_put_contents($output_file, $input_file, FILE_USE_INCLUDE_PATH);
	exit(0);
}


// pole, do ktereho se ulozi vysledek funkce preg_match
$keyword;
// regularni vyraz popisujici vsechny validni formatovaci prikazy
$valid_tags = "/^bold$|^italic$|^underline$|^teletype$|^size:[1-7]$|^color:[0-9A-Fa-f]{6}$/";

// overeni spravnosti formatovacich prikazu
// kontrola unikatnosti kazdeho prikazu na jednom radku
foreach ($tags as $fmt_expr => $tags_on_line)
{	
	$matched_tags = "";
	foreach ($tags_on_line as $tags_array => $s_tag)
	{
		if (preg_match($valid_tags, $s_tag, $keyword))
		{
			$abbrev = $s_tag[0].$s_tag[1].$s_tag[2];
			if (strpos($matched_tags, $abbrev) === FALSE)
				$matched_tags = $matched_tags . $keyword[0][0] . $keyword[0][1] . $keyword[0][2];
			else
			{
				print_error("Chyba ve formatovacim souboru, duplicitni formatovaci prikaz!\n");
				exit(4);
			}
		}
		else
		{
			print_error("Chyba ve formatovacim souboru, nevalidni formatovaci prikaz!\n");
			exit(4);			
		}

	}		
}


// prochazeni rv znak po znaku a overeni validnosti regularnich vyrazu
// kontrola poctu zavorek a spravnosti sousednich symbolu
$correct = TRUE;
foreach ($reg_exps as $index => $s_reg_exp)
{
	$prev_char = " ";
	$strlen = strlen($s_reg_exp);
	for ($i = 0; $i < $strlen; $i++)
	{
		$strlen = strlen($s_reg_exp);
		$char = substr($s_reg_exp, $i, 1);

		// znak "|" nesmi byt prvni ani posledni, nesmi nasledovat: ), |, +, *, .
		if (preg_match("/[\|]/", $char) && $prev_char != '%')
		{
			if (($i + 1 >= $strlen) || ($i == 0))
				$correct = FALSE;
			else
			{
				$char = substr($s_reg_exp, $i + 1, 1);
				if (preg_match("/[\)\|+*.]/", $char))
					$correct = FALSE;
			}
		}

		// znak "." nesmi byt prvni ani posledni, nesmi nasledovat: |, ., +, *, )
		else if (preg_match("/[.]/", $char) && $prev_char != '%')
		{
			if (($i + 1 >= $strlen) || ($i == 0))
				$correct = FALSE;
			else
			{
				$char = substr($s_reg_exp, $i + 1, 1);
				if (preg_match("/[\|.+*\)]/", $char))
					$correct = FALSE;
			}
		}

		// znak "+" nesmi byt prvni, pokud nasleduje + nebo *, smaze se prvni znak
		else if (preg_match("/[+]/", $char) && $prev_char != '%')
		{
			if ($i == 0)
				$correct = FALSE;
			if ($i + 1 >= $strlen)
				break;
			$char = substr($s_reg_exp, $i + 1, 1);
			if ($char == '+' || $char == '*')
			{
				$reg_exps[$index] = substr($s_reg_exp, 0, $i) . substr($s_reg_exp, $i + 1);
				$s_reg_exp = $reg_exps[$index];
				$i = $i - 1;				
			}
		}

		// znak "*" nesmi byt prvni, pokud nasleduje + nebo *, smaze se druhy znak
		else if (preg_match("/[*]/", $char) && $prev_char != '%')
		{
			if ($i == 0)
				$correct = FALSE;
			if ($i + 1 >= $strlen)
				break;
			$char = substr($s_reg_exp, $i + 1, 1);
			if ($char == '+' || $char == '*')
			{
				$reg_exps[$index] = substr($s_reg_exp, 0, $i + 1) . substr($s_reg_exp, $i + 2);
				$s_reg_exp = $reg_exps[$index];
				$i = $i - 1;			
			}
		}

		// znak "(" nesmi byt posledni, nesmi nasledovat: |, ), ., +, *
		else if (preg_match("/[\(]/", $char) && $prev_char != '%')
		{
			if ($i + 1 >= $strlen)
				$correct = FALSE;
			else
			{
				$char = substr($s_reg_exp, $i + 1, 1);
				if (preg_match("/[\|\).+*]/", $char))
					$correct = FALSE;
			}
		}

		if ($correct == FALSE)
			break;

		$prev_char = $char;
	}
}

if ($correct == FALSE)
{
	print_error("Chyba ve formatovacim souboru, nevalidni regularni vyraz!\n");
	exit(4);
}


// php ekvivalent za operator "!"
$replacement = "(?:(?!";
// pole znaku, ktere jsou potreba nahradit
$search = array("\"", "/", "$", "^", "{", "}", "=", "<", ">");
// pole znaku, za ktere maji byt nahrazeny
$replace = array("\\\"", "\/", "[$]", "\^", "\{", "\}", "\=", "\<", "\>");

// prochazeni rv znak po znaku, zpracovani regularnich 
// vyrazu, prelozeni na reg. vyrazy platne pro php
// plus kontrola validnosti dalsiho znaku po operatoru !
foreach ($reg_exps as $index => $expression)
{
	// nahrazeni znaku \ a -
	$expression = str_replace("\\", "\\\\", $expression);
	$expression = str_replace("-", "\-", $expression);

	$aux = "";
	$prev_char = "";
	$strlen = strlen($expression);

	// prvni pruchod znak po znaku
	for ($i = 0; $i < $strlen; $i++)
	{
		$char = substr($expression, $i, 1);

		// vynechani operatoru konkatenace
		if ($char == '.')
		{
			if (($prev_char == '%') || ($prev_char == '!'))
				$aux = $aux . $char;
		}

		// predelani operatoru ?, :, [, ]
		else if ($char == '?')
			$aux = $aux . "\?";
		else if ($char == ':')
			$aux = $aux . "\:";
		else if ($char == '[')
			$aux = $aux . "\[";
		else if ($char == ']')
			$aux = $aux . "\]";
		else
			$aux = $aux . $char;

		$prev_char = $char;
	}

	$expression = $aux;

	$aux = "";
	$prev_char = "";
	$strlen = strlen($expression);

	// druhy pruchod znak po znaku
	for ($i = 0; $i < $strlen; $i++)
	{
		$char = substr($expression, $i, 1);

		// pro operator !
		if (($char == '!') && ($prev_char != '%'))
		{
			if ($i + 1 >= $strlen)
			{
				print_error("Chyba ve formatovacim souboru, nevalidni regularni vyraz!\n");
				exit(4);
			}
			else
			{
				$i = $i + 1;
				$char = substr($expression, $i, 1);
				// pokud nasleduje znak %, nesmi to byt posledni znak
				if ($char == '%')
				{
					if ($i + 1 >= $strlen)
					{
						print_error("Chyba ve formatovacim souboru, nevalidni regularni vyraz!\n");
						exit(4);
					}
					else
					{
						$i = $i + 1;
						$next_char = substr($expression, $i, 1);
						if (preg_match("/[.|!*+()%]/", $next_char))
							$aux = $aux . "(?:(?!" . "[" . $next_char . "]" . ").)";
						else if (preg_match("/[sadlLwWtn]/", $next_char))
							$aux = $aux . "(?:(?!" . "%" . $next_char . ").)";

						else
						{			
							print_error("Chyba ve formatovacim souboru, nevalidni regularni vyraz!\n");
							exit(4);
						}
					}
				}

				// pokud nasleduje znak !, poscitaji se vsechny vykricniky a nahradi jednim, nebo zadnym
				else if ($char == '!')
				{
					$count = 1;
					do
					{
						$count = $count + 1;
						if ($i + 1 >= $strlen)
						{
							print_error("Chyba ve formatovacim souboru, nevalidni regularni vyraz!\n");
							exit(4);
						}
						$i = $i + 1;
						$char = substr($expression, $i, 1);
					} while ($char == '!');
					if ($count % 2)
						$i = $i - 2;
					else
						$aux = $aux . $char;
				}

				// pokud nasleduje znak (, nacte se cely vyraz v zavorkach
				else if ($char == '(')
				{
					$brace_count = 1;
					$substring = "(";
					$aux = $aux . "(?:(?!";
					do {
						if ($i + 1 >= $strlen)
						{
							print_error("Chyba ve formatovacim souboru, nevalidni regularni vyraz!\n");
							exit(4);
						}
						$i = $i + 1;
						$char = substr($expression, $i, 1);
						$substring = $substring . $char;
						if ($char == ')' && $prev_char != '%')
							$brace_count = $brace_count - 1;
						if ($char == '(' && $prev_char != '%')
							$brace_count = $brace_count + 1;
						$prev_char = $char;
					} while ($brace_count != 0);

					// rv na overeni, ze prochazeny rv popisuje vzdy maximalne jeden znak
					$string = "/^(\()+(.|%[sadlLwWtn.|!*+()%])(\|(\()*(.|%[sadlLwWtn.|!*+()%])|(\))*\|(.|%[sadlLwWtn.|!*+()%]))*(\))+$/";
					if (preg_match($string, $substring, $found))
						$aux = $aux . $substring . ").)";
					else
					{
						print_error("Chyba ve formatovacim souboru, nevalidni regularni vyraz!\n");
						exit(4);
					}
				}
				else if (preg_match("/[*+\)\|.]/", $char))
				{
					print_error("Chyba ve formatovacim souboru, nevalidni regularni vyraz!\n");
					exit(4);
				}
				else
					$aux = $aux . "(?:(?!" . $char . ").)";
			}
		}
		else
			$aux = $aux . $char;
		$prev_char = $char;
	}


	$expression = $aux;
	$aux = "";
	$prev_char = "";
	$strlen = strlen($expression);

	// treti pruchod znak po znaku na zpracovani vsech vyrazu s %
	for ($i = 0; $i < $strlen; $i++)
	{
		$char = substr($expression, $i, 1);
		if ($char == '%')
		{
			if ($i + 1 >= $strlen)
			{
				print_error("Chyba ve formatovacim souboru, nevalidni regularni vyraz!\n");
				exit(4);
			}
			else
			{
				$count = 0;
				do
				{
					$count = $count + 1;
					if ($i + 1 >= $strlen)
					{
						if ($count % 2)
						{
							print_error("Chyba ve formatovacim souboru, nevalidni regularni vyraz!\n");
							exit(4);
						}
						else
							break;
					}
					$i = $i + 1;
					$char = substr($expression, $i, 1);
				} while ($char == '%');
		
				if ($count % 2) 
				{
					for ($it = 0; $it < ($count - 1)/2; $it++) 
						$aux = $aux . "[%]";							
					if (preg_match("/[+*!\|.]/", $char))
						$aux = $aux . "[" . $char . "]";
					else if (preg_match("/[\(\)sdtn]/", $char))
						$aux = $aux . "\\" . $char;
					else if ($char == 'a')
						$aux = $aux . "[\s|\S]";						
					else if ($char == 'l')
						$aux = $aux . "[a-z]";						
					else if ($char == 'L')
						$aux = $aux . "[A-Z]";
					else if ($char == 'w')
						$aux = $aux . "[a-zA-Z]";
					else if ($char == 'W')
						$aux = $aux . "[a-zA-Z0-9]";
				}

				else 
				{
					for ($it = 0; $it < $count/2; $it++)
						$aux = $aux . "[%]";
					if ($char != '%')
						$i = $i - 1;
				}

			}
		}
		else
			$aux = $aux . $char;

		$prev_char = $char;
	}


	$expression = $aux;
	$count = 0;
	$prev_char = "";
	$strlen = strlen($expression);

	// ctvrty pruchod znak po znaku k overeni poctu a poradi zavorek
	for ($i = 0; $i < $strlen; $i++)
	{
		$char = substr($expression, $i, 1);
		if (($char == '(') && $prev_char != '\\')
			$count = $count + 1;
		else if (($char == ')') && $prev_char != '\\')
			$count = $count - 1;

		if ($count < 0)
			break;

		$prev_char = $char;
	}

	if ($count != 0)
	{
		print_error("Chyba ve formatovacim souboru!\n");
		exit(4);
	}


	$reg_exps[$index] = str_replace($search, $replace, $aux);
	$reg_exps[$index] = '/' .  $reg_exps[$index] . '/';
}


// vyhledani casti textu, ktere odpovidaji regularnim vyrazum
// ulozeni podretezcu a pozic pocatku do pole matches
// odstraneni nalezenych prazdnych retezcu a duplicit
$matches = array();
$found = array();
foreach ($reg_exps as $index => $pattern)
{
	if ((preg_match_all($pattern, $input_file, $found, PREG_OFFSET_CAPTURE)) === FALSE)
	{
		print_error("Chyba ve formatovacim souboru!\n");
		exit(4);
	}

	// odstraneni vsech nalezenych podretezcu, ktere jsou prazdne
	for ($i = sizeof($found) - 1; $i >= 0 ; $i--)
	{
		for ($j = sizeof($found[$i]) - 1; $j >= 0; $j--)
		{
			if (empty($found[$i][$j]))
				unset($found[$i][$j]);
		}
		$found[$i] = array_values($found[$i]);
	}

	// odstraneni podretezcu, ktere se vyskytuji vice nez jednou pro jeden rv
	for ($i = sizeof($found) - 1; $i >= 0 ; $i--)
	{
		for ($j = sizeof($found[$i]) - 1; $j >= 0; $j--)
		{
			if (($j > 0) && ($found[$i][$j][0] == $found[$i][$j-1][0]) && ($found[$i][$j][1] == $found[$i][$j-1][1]))
				unset($found[$i][$j]);
			else if ((!preg_match($pattern, $found[$i][$j][0])) || ($found[$i][$j][0] == ""))
				unset($found[$i][$j]);
			else if (($i > 0) && ($found[$i][$j][0] == $found[$i-1][$j][0]) && ($found[$i][$j][1] == $found[$i-1][$j][1]))
				unset($found[$i][$j]);
		}
	}
	$matches[] = $found;
}


// precislovani pole uchovavajiciho nalezene casti vstupniho souboru
for ($i = 0; $i < sizeof($matches); $i++)
{
	$matches[$i] = array_values($matches[$i]);
	for ($j = 0; $j < sizeof($matches[$i]); $j++)
	{
		$matches[$i][$j] = array_values($matches[$i][$j]);
		for ($k = 0; $k < sizeof($matches[$i][$j]); $k++)
		{
			$matches[$i][$j][$k] = array_values($matches[$i][$j][$k]);
		}
	}
}


// rozsireni pole matches o umisteni konce formatovaciho prikazu
for ($i = 0; $i < sizeof($matches); $i++)
{
	for ($j = 0; $j < sizeof($matches[$i]); $j++)
	{
		for ($k = 0; $k < sizeof($matches[$i][$j]); $k++)
		{
			if (isset($matches[$i][$j][$k]))
				$matches[$i][$j][$k][] = $matches[$i][$j][$k][1] + strlen($matches[$i][$j][$k][0]);
		}
	}
}


// pridani formatovacich znacek do vstupniho textu
// nalezeni nejnizsi pozice znacky, vlozeni do puvodniho textu
// a nasledne prepocitani vsech vyssich (nebo rovnych) indexu
do
{
	// nalezeni nejnizsi pozice formatovaci znacky
	$min = INF;
	for ($i = 0; $i < sizeof($matches); $i++)
	{
		for ($j = 0; $j < sizeof($matches[$i]); $j++)
		{
			for ($k = 0; $k < sizeof($matches[$i][$j]); $k++)
			{
				if ($matches[$i][$j][$k][1] < $min)
				{
					$min = $matches[$i][$j][$k][1];
					$pos = array($i, $j, $k, 1);
				}
				if ($matches[$i][$j][$k][2] <= $min)
				{
					$min = $matches[$i][$j][$k][2];
					$pos = array($i, $j, $k, 2);
				}
			}
		}
	}
	if ($min === INF)
		break;

	// jestlize se jedna o pocatecni znacku
	// projdi ulozene formatovaci prikazy a vloz do vstupniho textu
	if ($pos[3] == 1)
	{
		for ($i = sizeof($tags[$pos[0]]) - 1; $i >= 0; $i--)
		{
			if ($tags[$pos[0]][$i] == "bold")
				$add_string = "<b>";
			else if ($tags[$pos[0]][$i] == "italic")
				$add_string = "<i>";
			else if ($tags[$pos[0]][$i] == "underline")
				$add_string = "<u>";
			else if ($tags[$pos[0]][$i] == "teletype")
				$add_string = "<tt>";
			else if (preg_match("/size:./", $tags[$pos[0]][$i]))
			{
				$num = substr($tags[$pos[0]][$i], 5, 1);
				$add_string = "<font size=" . $num . ">";
			}
			else
			{
				$hex = substr($tags[$pos[0]][$i], 6, 6);
				$add_string = "<font color=#" . $hex . ">";
			}

			$input_file = substr($input_file, 0, $min) . $add_string . substr($input_file, $min);
		
			// prepocitani dalsich indexu
			for ($it = 0; $it < sizeof($matches); $it++)
			{
				for ($j = 0; $j < sizeof($matches[$it]); $j++)
				{
					for ($k = 0; $k < sizeof($matches[$it][$j]); $k++)
					{
						if ($matches[$it][$j][$k][1] >= $min)
							$matches[$it][$j][$k][1] += strlen($add_string);
						if ($matches[$it][$j][$k][2] >= $min)
							$matches[$it][$j][$k][2] += strlen($add_string);
					}
				}
			}

		}
		$matches[$pos[0]][$pos[1]][$pos[2]][$pos[3]] = INF;
	}
	
	// jestlize se jedna o ukoncovaci znacku
	// projdi ulozene formatovaci prikazy a vloz do vstupniho textu
	else
	{
		for ($i = 0; $i < sizeof($tags[$pos[0]]); $i++)
		{
			if ($tags[$pos[0]][$i] == "bold")
				$add_string = "</b>";
			else if ($tags[$pos[0]][$i] == "italic")
				$add_string = "</i>";
			else if ($tags[$pos[0]][$i] == "underline")
				$add_string = "</u>";
			else if ($tags[$pos[0]][$i] == "teletype")
				$add_string = "</tt>";
			else if (preg_match("/size:./", $tags[$pos[0]][$i]))
			{
				$num = substr($tags[$pos[0]][$i], 5, 1);
				$add_string = "</font>";
			}
			else
			{
				$hex = substr($tags[$pos[0]][$i], 6, 6);
				$add_string = "</font>";
			}

			$input_file = substr($input_file, 0, $min) . $add_string . substr($input_file, $min);

			// prepocitani dalsich indexu		
			for ($it = 0; $it < sizeof($matches); $it++)
			{
				for ($j = 0; $j < sizeof($matches[$it]); $j++)
				{
					for ($k = 0; $k < sizeof($matches[$it][$j]); $k++)
					{
						if ($matches[$it][$j][$k][1] >= $min)
							$matches[$it][$j][$k][1] += strlen($add_string);
						if ($matches[$it][$j][$k][2] >= $min)
							$matches[$it][$j][$k][2] += strlen($add_string);
					}
				}
			}

		}
		// oznaceni vybrane pozice znacky za pouzitou
		$matches[$pos[0]][$pos[1]][$pos[2]][$pos[3]] = INF;
	}

} while ($min !== INF);


// pridani znacky <br /> pred znak konce radku
if (isset($params["br"]))
	$input_file = str_replace("\n", "<br />\n", $input_file);

// zapsani obsahu do vystupniho souboru
file_put_contents($output_file, $input_file, FILE_USE_INCLUDE_PATH);

?>
