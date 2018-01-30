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
            function check_add_title()
            {
                var txt = document.getElementById("jmeno").value;
                if (txt.match(/^([a-z]|\s|ě|š|č|ř|ž|ý|á|í|é|ú|ů|ó|ň|ď|ť)*$/i))
                {
                    document.getElementById("jmeno").style.color = "black";
                    document.getElementById("submit").disabled = false;
                }
                else
                {
                    document.getElementById("submit").disabled = true;                     
                    document.getElementById("jmeno").style.color = "red";
                }       
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
        <h2 align="center">Správa uživatelů</h2><br>

        <?php if ($_SESSION['usertype'] > 0): ?>
        <a href="add_user_form.php"><p style="text-align:center"><font size="4.5">Přidat uživatele</font></p></a><br>
        <?php endif; ?> 

        <form action="search_users.php" method="POST">
        Vyhledat uživatele:
        <input type="text" name="search" id="jmeno" oninput="check_add_title()">&ensp;
        <input type="submit" value="Hledej" id="submit">
        </form>

        <?php if (isset($_SESSION['users']) && sizeof($_SESSION['users']) > 0): ?>
        <br><br>
        <table align="center">
        <tr>
            <th width="28%">Jméno</th>
            <th width="28%">Login</th>
            <th width="28%">Bydliště</th>
            <th width="16%">Typ uživatele</th>
        </tr>
        <?php foreach ($_SESSION['users'] as $single_row):?>
        <tr>
            <td><a href="user_data.php?UC=<?php echo $single_row['UC']; ?>"><?php echo $single_row['jmeno'] . " " . $single_row['prijmeni']; ?></a></td>
            <td><?php echo $single_row['login']; ?></td>
            <td><?php if ($single_row['ulice'] != "" && $single_row['mesto'] != "" && $single_row['psc'] != 0):
             echo $single_row['ulice'] . ", " . $single_row['mesto'] . ", " . $single_row['psc']; 
             else: echo "-"; endif; ?></td>
            <td><?php if ($single_row['typ_uziv'] == 0):
                    echo "čtenář"; 
                elseif ($single_row['typ_uziv'] == 1):
                    echo "knihovník";
                else:
                    echo "vedoucí";
                endif; ?></td>
        </tr>
        <?php endforeach; ?>
        </table>
        <?php endif; ?>

        <?php if (isset($_SESSION['users']) && sizeof($_SESSION['users']) == 0): ?>
        <br>
        <div align="center" role="alert"><strong>Podle zadaných kritérií nebyly nalezeny žádné záznamy.</strong></div><br>
        <?php unset($_SESSION['users']); 
        endif; ?>
    <br>
    </body>
</html>