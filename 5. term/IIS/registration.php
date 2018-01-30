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
            width: 30%;
            align: center;
        }

        td, th {
            text-align: left;
            padding: 8px;
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

                txt = document.getElementById("heslo").value;
                if (txt.length >= 4 && txt.length <= 20)
                {
                    document.getElementById("heslo").style.color = "black";
                }
                else
                {
                    ok = false;                       
                    if (txt.length == 0)
                        document.getElementById("heslo").style.color = "black";
                    else                            
                        document.getElementById("heslo").style.color = "red";
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

        <h1 align="center">Informační systém knihovny</h1>
        <h2 align="center">Registrace</h2><br>

        <?php if (isset($_SESSION['error']) && $_SESSION['error'] == '1'): ?>
        <br>
        <div align="center" role="alert"><strong>Vámi zadaný login je již využíván, zvolte jiný.</strong></div><br>
        <?php unset($_SESSION['error']);
        endif; ?>

        <form action="add_user.php" method="POST">              <!-- TODO check if all * fields inserted and check values -->
        <table align="center">
        <tr>
            <td>Jméno: </td>
            <td><input type="text" name="name" maxlength="255" id="jmeno" oninput="check_add_title()"> *</td>
        </tr><tr>
            <td>Příjmení: </td>
            <td><input type="text" name="surname" maxlength="255" id="prijmeni" oninput="check_add_title()"> *</td>
        </tr><tr>
            <td>Uživatelské jméno: </td>
            <td><input type="text" name="login" maxlength="50" id="login" oninput="check_add_title()"> *</td>
        </tr><tr>
            <td>Heslo: </td>
            <td><input type="password" name="passwd" maxlength="50" id="heslo" oninput="check_add_title()"> *</td>
        </tr><tr>
            <td>E-mail: </td>
            <td><input type="text" name="email" maxlength="50" id="mail" oninput="check_add_title()"></td>
        </tr><tr>
            <td>Ulice: </td>
            <td><input type="text" name="street" maxlength="50" id="ulice" oninput="check_add_title()"></td>
        </tr><tr>
            <td>Město: </td>
            <td><input type="text" name="city" maxlength="50" id="mesto" oninput="check_add_title()"></td>
        </tr><tr>
            <td>PSČ: </td>
            <td><input type="text" name="psc" maxlength="5" id="psc" oninput="check_add_title()"></td>
        </tr><tr>
            <td>Telefon: </td>
            <td><input type="text" name="phone" maxlength="13" id="tel" oninput="check_add_title()"></td>
        </tr><tr>
            <td> </td>
            <td>* povinná pole</td>
        </tr>
        </table>
        <br>
        <input type="submit" value="Zaregistrovat" id="submit" disabled>
        <input type="hidden" name="user_type" value="0">
        <input type="hidden" value="intraportal.php" name="next_page">
        </form>
    <br>
    </body>
</html> 