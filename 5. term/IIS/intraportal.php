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
                font-family: arial, sans-serif;
                border-collapse: collapse;
                width: 70%;
                align: center;
            }

            td, th {
                border: 1px solid #dddddd;
                text-align: left;
                padding: 8px;
            }

            tr:nth-child(even) {
                background-color: #dddddd;
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
            function check()
            {
                var txt = document.getElementById("txt").value;
                if (document.getElementById("isbn").checked)
                {
                    if (txt.match(/^[0-9]*$/i) && is_isbn_issn(txt))
                    {
                        document.getElementById("submit").disabled = false;
                        document.getElementById("txt").style.color = "black";
                    }
                    else
                    {
                        document.getElementById("submit").disabled = true;
                        if (txt.length == 0)
                            document.getElementById("txt").style.color = "black";
                        else                            
                            document.getElementById("txt").style.color = "red";
                    }
                }
                else if (document.getElementById("nazev").checked)
                {
                    if (txt.match(/^([0-9a-z]|\s|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť)*$/i))
                    {
                        document.getElementById("submit").disabled = false;
                        document.getElementById("txt").style.color = "black";
                    }
                    else
                    {
                        document.getElementById("submit").disabled = true;
                        document.getElementById("txt").style.color = "red";
                    }
                }
                else
                {
                    if (txt.match(/^([a-z]|\s|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť)*$/i))
                    {
                        document.getElementById("submit").disabled = false;
                        document.getElementById("txt").style.color = "black";
                    }
                    else
                    {
                        document.getElementById("submit").disabled = true;
                        document.getElementById("txt").style.color = "red";
                    }
                }
            }
            function is_isbn_issn(txt)
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
                else if (txt.length == 8)
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
        <h2 align="center">Intraportál</h2><br>

        <?php if ($_SESSION['usertype'] > 0): ?>
        <a href="add_title.php"><p style="text-align:center"><font size="4.5">Přidat titul</font></p></a><br>
        <?php endif; ?> 

        <form action="search_db.php" method="POST">
        Vyhledat titul:
        <input type="text" name="search" id="txt" oninput="check()">&ensp;
        <input type="radio" name="attribute" value="nazev" id="nazev" oninput="check()" checked> Název
        <input type="radio" name="attribute" value="autor" id="autor" oninput="check()"> Autor
        <input type="radio" name="attribute" value="zanr" id="zanr" oninput="check()"> Žánr
        <input type="radio" name="attribute" value="isbn" id="isbn" oninput="check()"> ISBN &emsp;
        <input type="submit" value="Hledej" id="submit">
        </form>

        <?php if (isset($_SESSION['register']) && $_SESSION['register'] == '1'): ?>
        <br>
        <div align="center" role="alert"><strong>Registrace proběhla úspěšně.</strong></div><br>
        <?php unset($_SESSION['register']);
        endif; ?>

        <?php if (isset($_SESSION['rows']) && sizeof($_SESSION['rows']) > 0): ?>
        <br><br>
        <form action="add_reservation.php" method="POST">
        <table align="center">
        <tr>
            <th width="28%">Název</th>
            <th width="28%">Autor</th>
            <th width="28%">Žánr</th>
            <th width="16%">Rezervovat</th>
        </tr>
        <?php foreach ($_SESSION['rows'] as $single_row):?>
        <tr>
            <td><?php if ($_SESSION['usertype'] > 0): ?>
                <a href="get_title_info.php?TID=<?php echo $single_row['TID']; ?>"><?php echo $single_row['Tnazev']; ?></a>
                <?php else: echo $single_row['Tnazev']; ?>
            <?php endif; ?></td>
            <td><?php echo $single_row['Ajmeno'] . " " . $single_row['Aprijmeni']; ?></td>
            <td><?php echo $single_row['zanr']; ?></td>
            <td><?php if ($single_row['is_reserved'] == 0): ?> 
                <input type="submit" name="<?php echo $single_row['TID']; ?>" value="Rezervovat">
            <?php else: ?>
                Rezervováno
            <?php endif; ?> </td>
        </tr>
        <?php endforeach; ?>
        </table>
        </form>
        <?php endif; ?>

        <?php if (isset($_SESSION['rows']) && sizeof($_SESSION['rows']) == 0): ?>
        <br>
        <div align="center" role="alert"><strong>Podle zadaných kritérií nebyly nalezeny žádné záznamy.</strong></div><br>
        <?php unset($_SESSION['rows']); 
        endif; ?>
    <br>
    </body>
</html>