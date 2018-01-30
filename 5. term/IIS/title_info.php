<?php
    session_start();
    header("Content-Type: text/html; charset=UTF-8");
?>
<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0" charset="UTF-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title>Knihovna</title>

        <style>
            form {
                text-align: center;
            }

            table {
                border-collapse: collapse;
                width: 28%;
                align: center;
            }

            td, th {
                text-align: left;
                padding: 8px;
            }

            ul {
                list-style-type: none;
                margin: 0;
                padding: 4;
                overflow: hidden;
                background-color: #DDDDDD;
            }

            li {
                float: left;
                margin: 0; padding: 0;
            }

            li a {
                display: block;
                color: black;
                text-align: center;
                padding: 16px;
                text-decoration: none;
            }

            li a:hover {
                background-color: #AAAAAA;
            }
        </style>
        <script>
            function check_add_title()
            {
                var ok = true;

                var txt = document.getElementById("nazev").value;
                if (txt.match(/^([0-9a-z]|\s|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť)+$/i))
                {
                    document.getElementById("nazev").style.color = "black";
                }
                else
                {
                    ok = false;
                    if (txt.length == 0)
                        document.getElementById("nazev").style.color = "black";
                    else                            
                        document.getElementById("nazev").style.color = "red";
                }

                txt = document.getElementById("jmeno").value;
                if (txt.match(/^([a-z]|\s|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť)+$/i))
                {
                    document.getElementById("jmeno").style.color = "black";
                }
                else
                {
                    ok = false;
                    if (txt.length == 0)
                        document.getElementById("jmeno").style.color = "black";
                    else                            
                        document.getElementById("jmeno").style.color = "red";
                }

                txt = document.getElementById("prijmeni").value;
                if (txt.match(/^([a-z]|\s|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť)+$/i))
                {
                    document.getElementById("prijmeni").style.color = "black";
                }
                else
                {
                    ok = false;
                    if (txt.length == 0)
                        document.getElementById("prijmeni").style.color = "black";
                    else                            
                        document.getElementById("prijmeni").style.color = "red";
                }

                txt = document.getElementById("jazyk").value;
                if (txt.match(/^([a-z]|\s|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť)*$/i))
                {
                    document.getElementById("jazyk").style.color = "black";
                }
                else
                {
                    ok = false;                       
                    document.getElementById("jazyk").style.color = "red";
                }

                txt = document.getElementById("zanr").value;
                if (txt.match(/^([a-z]|\s|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť)*$/i))
                {
                    document.getElementById("zanr").style.color = "black";
                }
                else
                {
                    ok = false;                       
                    document.getElementById("zanr").style.color = "red";
                }

                txt = document.getElementById("isbn_issn").value;
                if (document.getElementById("type").innerHTML == "ISBN")
                {
                    if (txt.length == 0 || (txt.match(/^[0-9]*$/i) && is_isbn(txt)))
                    {
                        document.getElementById("isbn_issn").style.color = "black";
                    }
                    else
                    {
                        ok = false;                       
                        document.getElementById("isbn_issn").style.color = "red";
                    }
                }
                else
                {
                    if (txt.length == 0 || (txt.match(/^[0-9]*$/i) && is_issn(txt)))
                    {
                        document.getElementById("isbn_issn").style.color = "black";
                    }
                    else
                    {
                        ok = false;                       
                        document.getElementById("isbn_issn").style.color = "red";
                    }
                }

                if (ok)
                    document.getElementById("submit").disabled = false;
                else
                    document.getElementById("submit").disabled = true;               
            }
            function is_isbn(txt)
            {
                var count = 0;
                if (txt.length == 13)
                {
                    for (var i = 0; i < 13; i++)
                    {
                        if (i % 2 == 0)
                            count += Number(txt[i]);
                        else                            
                            count += 3 * Number(txt[i]);
                    }
                    if (count % 10 == 0)
                        return true;
                }
                else if (txt.length == 10)
                {
                    for (var i = 0; i < 10; i++)
                    {                   
                        count += (10 - i) * Number(txt[i]);
                    }
                    if (count % 11 == 0)
                        return true;
                }
                return false;
            }
            function is_issn(txt)
            {
                var count = 0;
                if (txt.length == 8)
                {
                    for (var i = 0; i < 8; i++)
                    {                   
                        count += (8 - i) * Number(txt[i]);
                    }
                    if (count % 11 == 0)
                        return true;
                }
                return false;
            }
        </script>

    </head>
    <body>
        <?php if (empty($_SESSION)):
            $_SESSION['error'] = "3";
            header('Location: index.php');
        endif; ?>
        
        <ul>
            <?php if ($_SESSION['usertype'] > 0): ?>
                <li><a href="intraportal.php">Správa titulů</a></li>
                <li><a href="users_db.php">Správa uživatelů</a></li>
            <?php else: ?>
                <li><a href="intraportal.php">Vyhledání titulů</a></li>
            <?php endif; ?> 
            <li><a href="get_user_info.php">Můj účet</a></li>
            <li><a href="logout.php">Odhlásit</a></li>
        </ul>
        <br>
        <h1 align="center">Informační systém knihovny</h1>
        <h2 align="center">Správa titulu</h2><br>

        <?php if (isset($_SESSION['error']) && $_SESSION['error'] == 1): ?>
            <div align="center" role="alert"><strong>Chyba při aktualizaci údajů, zadány neplatné hodnoty.</strong></div><br>
        <?php unset($_SESSION['error']);
        elseif (isset($_SESSION['error']) && $_SESSION['error'] == 2): ?>
            <div align="center" role="alert"><strong>Chyba při odstraňování záznamu.</strong></div><br>
        <?php unset($_SESSION['error']);
        elseif (isset($_SESSION['error']) && $_SESSION['error'] == 0): ?>
            <div align="center" role="alert"><strong>Editace proběhla úspěšně.</strong></div><br>
        <?php unset($_SESSION['error']); endif; ?>

        <br><br>
        <form action="title_db_op.php" method="POST">
        <table align="center">
        <tr>
            <td width="65%">Název</td>
            <td width="35%"><input type="text" maxlength="50" name="title" id="nazev" oninput="check_add_title()" value="<?php echo $_SESSION['title_info']['Tnazev']; ?>"</td>
        </tr>
        <tr>
            <td>Jméno autora</td>
            <td><input type="text" maxlength="50" name="aname" id="jmeno" oninput="check_add_title()" value="<?php echo $_SESSION['title_info']['Ajmeno']; ?>"</td>
         </tr>
        <tr>
            <td>Příjmení autora</td>
            <td><input type="text" maxlength="50" name="asurname" id="prijmeni" oninput="check_add_title()" value="<?php echo $_SESSION['title_info']['Aprijmeni']; ?>"</td>
         </tr>
         <tr>
            <td>Žánr</td>
            <td><input type="text" maxlength="20" name="genre" id="zanr" oninput="check_add_title()" value="<?php echo $_SESSION['title_info']['zanr']; ?>"</td>
        </tr>
         <tr>
            <td>Jazyk</td>
            <td><input type="text" maxlength="20" name="language" id="jazyk" oninput="check_add_title()" value="<?php echo $_SESSION['title_info']['jazyk']; ?>"</td>
         <tr>
            <?php if ($_SESSION['title_info']['ISBN'] != NULL): ?>
                <td id="type">ISBN</td>
                <td><input type="text" maxlength="13" name="isbn" id="isbn_issn" oninput="check_add_title()" value="<?php echo $_SESSION['title_info']['ISBN']; ?>"</td>
            <?php else: ?>
                <td id="type">ISSN</td>
                <td><input type="text" maxlength="8" name="issn" id="isbn_issn" oninput="check_add_title()" value="<?php echo $_SESSION['title_info']['ISSN']; ?>"</td>
            <?php endif; ?>
        </tr>
        </table>
        <br><input type="submit" value="Uložit změny" name="save" id="submit">&emsp;
        <input type="submit" value="Odstranit titul" name="remove">
        <input type="hidden" value="<?php echo $_SESSION['title_info']['TID'];?>" name="titulID">
        <input type="hidden" value="<?php echo $_SESSION['title_info']['AC'];?>" name="AC">
        </form>
        <br>
    </body>
</html>