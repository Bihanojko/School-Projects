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

                txt = document.getElementById("login").value;
                if (txt.match(/^([0-9a-z])+$/))
                {
                    document.getElementById("login").style.color = "black";
                }
                else
                {
                    ok = false;                       
                    if (txt.length == 0)
                        document.getElementById("login").style.color = "black";
                    else                            
                        document.getElementById("login").style.color = "red";
                }

                txt = document.getElementById("ulice").value;
                if (txt.match(/^([0-9a-z]|\s|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť)*$/i))
                {
                    document.getElementById("ulice").style.color = "black";
                }
                else
                {
                    ok = false;                 
                    document.getElementById("ulice").style.color = "red";
                }

                txt = document.getElementById("mesto").value;
                if (txt.match(/^([a-z]|\s|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť)*$/i))
                {
                    document.getElementById("mesto").style.color = "black";
                }
                else
                {
                    ok = false;                      
                    document.getElementById("mesto").style.color = "red";
                }

                txt = document.getElementById("psc").value;
                if (txt.match(/^([0-9])*$/i) && (txt.length == 5 || txt.length == 0))
                {
                    document.getElementById("psc").style.color = "black";
                }
                else
                {
                    ok = false;                      
                    document.getElementById("psc").style.color = "red";
                }

                txt = document.getElementById("mail").value;
                if (txt.match(/^([0-9a-z]|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť|@|\.)*$/))
                {
                    document.getElementById("mail").style.color = "black";
                }
                else
                {
                    ok = false;                      
                    document.getElementById("mail").style.color = "red";
                }

                txt = document.getElementById("tel").value;
                if (txt.match(/^(([0-9]{9})|(\+([0-9]{12})))?$/))
                {
                    document.getElementById("tel").style.color = "black";
                }
                else
                {
                    ok = false;                                             
                    if (txt.length == 0)
                        document.getElementById("tel").style.color = "black";
                    else                            
                        document.getElementById("tel").style.color = "red";
                }

                

                if (ok)
                    document.getElementById("submit").disabled = false;
                else
                    document.getElementById("submit").disabled = true;               
            }
        </script>

    </head>
    <body>
        <?php if (empty($_SESSION)):
            $_SESSION['error'] = "3";
            header('Location: index.php');
        endif; ?>

        <?php if (isset($_GET['UC'])): $UC = $_GET['UC']; endif; ?>
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
        <h2 align="center">Správa uživatele</h2><br>

        <?php if (isset($_SESSION['error']) && $_SESSION['error'] == 1): ?>
            <div align="center" role="alert"><strong>Chyba při aktualizaci údajů, zadány neplatné hodnoty.</strong></div><br>
        <?php unset($_SESSION['error']);
        elseif (isset($_SESSION['error']) && $_SESSION['error'] == 2): ?>
            <div align="center" role="alert"><strong>Chyba při odstraňování záznamu.</strong></div><br>
        <?php unset($_SESSION['error']);
        elseif (isset($_SESSION['error']) && $_SESSION['error'] == 4): ?>
            <div align="center" role="alert"><strong>Výpůjčka byla úspěšně vytvořena.</strong></div><br>
        <?php unset($_SESSION['error']);
        elseif (isset($_SESSION['error']) && $_SESSION['error'] == 0): ?>
            <div align="center" role="alert"><strong>Editace proběhla úspěšně.</strong></div><br>
        <?php unset($_SESSION['error']); endif; ?>

        <?php foreach ($_SESSION['users'] as $single_row):
            if (isset($_GET['UC']) && $single_row['UC'] == $UC):
                $row = $single_row;
            endif; endforeach; ?>
        
        <br>
        <form action="user_db_op.php" method="POST"> 
        <table align="center" width="28%">
        <tr>
            <td width="65%">Jméno</td>
            <td width="35%"><input type="text" maxlength="255" name="name" id="jmeno" oninput="check_add_title()" value="<?php echo $row['jmeno']; ?>"</td>
        </tr>
        <tr>
            <td>Příjmení</td>
            <td><input type="text" maxlength="255" name="surname" id="prijmeni" oninput="check_add_title()" value="<?php echo $row['prijmeni']; ?>"</td>
         </tr>
        <tr>
            <td>Login</td>
            <td><input type="text" maxlength="50" name="login" id="login" oninput="check_add_title()" value="<?php echo $row['login']; ?>"</td>
         </tr>
         <tr>
            <td>Typ</td>
            <td><input type="radio" name="user_type" value="0" <?php if ($row['typ_uziv'] == 0): echo "checked"; endif; ?> > čtenář
            <input type="radio" name="user_type" value="1" <?php if ($row['typ_uziv'] == 1): echo "checked"; endif; ?>> knihovník
            <?php if ($_SESSION['usertype'] > 1): ?>
            <input type="radio" name="user_type" value="2" <?php if ($row['typ_uziv'] == 2): echo "checked"; endif; ?>> vedoucí<?php endif;?></td>
        </tr>
         <tr>
            <td>Ulice</td>
            <td><input type="text" maxlength="50" name="street" id="ulice" oninput="check_add_title()" value="<?php echo $row['ulice']; ?>"</td>
        </tr>
         <tr>
            <td>Město</td>
            <td><input type="text" maxlength="50" name="city" id="mesto" oninput="check_add_title()" value="<?php echo $row['mesto']; ?>"</td>
         </tr>
         <tr>
            <td>PSČ</td>
            <td><input type="text" maxlength="6" name="psc" id="psc" oninput="check_add_title()" value="<?php echo $row['psc']; ?>"</td>
         </tr>
         <tr>
            <td>E-mail</td>
            <td><input type="text" maxlength="50" name="email" id="mail" oninput="check_add_title()" value="<?php echo $row['email']; ?>"</td>
         </tr>
         <tr>
            <td>Telefon</td>
            <td><input type="text" maxlength="13" name="phone" id="tel" oninput="check_add_title()" value="<?php echo $row['tel']; ?>"</td>
         </tr>
        </table>
        <br><input type="submit" value="Uložit změny" name="save" id="submit">&emsp;
        <input type="submit" value="Odstranit uživatele" name="remove">
        <input type="hidden" value="<?php echo $row['UC'];?>" name="UC">
        </form>
        <br>

        <?php if (isset($row['borrowings']) && sizeof($row['borrowings']) == 0): ?>
        <br><div align="center" role="alert">Uživatel nemá vytvořené žádné výpůjčky.</div><br>
        <?php else: ?>

        <form action="remove_borrowing.php" method="POST">
        <h3 align="center">Výpůjčky</h3>
        <a href=<?php echo "add_borrowing.php?UC=".$UC; ?>><p style="text-align:center"><font size="4.5">Vytvořit výpůjčku</font></p></a><br>
        <table align="center" width="70%" class="duo">
        <tr>
            <th width="28%">Název</th>
            <th width="28%">Autor</th>
            <th width="17%">Vypůjčeno dne</th>
            <th width="17%">Vráceno dne</th>
            <th width="10%">Stav</th>
        </tr>
        <?php foreach ($row['borrowings'] as $borrowing): ?>
        <tr>
            <td><?php echo $borrowing['Tnazev']; ?></td>
            <td><?php echo $borrowing['Ajmeno'] . " " . $borrowing['Aprijmeni']; ?></td>
            <td><?php echo $borrowing['datum_vypujceni']; ?></td>
            <td><?php if ($borrowing['datum_vraceni'] == NULL): echo "-"; else: echo $borrowing['datum_vraceni']; endif; ?></td>
            <td><?php if ($borrowing['stav'] == 0): echo "vráceno"; else: ?><input type="submit" name="<?php echo $borrowing['VID']; ?>" value="Uzavřít"> <?php endif; ?></td>
       </tr>
        <?php endforeach; ?>
        </table>
        </form>

        <?php endif; ?>
        <br><br>

    </body>
</html>