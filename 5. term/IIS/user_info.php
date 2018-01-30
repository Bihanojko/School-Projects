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
        <h2 align="center">Můj účet</h2><br>

        <?php if (isset($_SESSION['reservations']) && sizeof($_SESSION['reservations']) > 0): ?>
        <h3 align="center">Rezervace</h3>
        <form action="remove_reservation.php" method="POST">
        <table align="center">
        <tr>
            <th width="35%">Název</th>
            <th width="35%">Autor</th>
            <th width="30%">Zrušit rezervaci</th>
        </tr>
        <?php foreach ($_SESSION['reservations'] as $single_row): ?>
        <tr>
            <td><?php echo $single_row['Tnazev']; ?></td>
            <td><?php echo $single_row['Ajmeno'] . " " . $single_row['Aprijmeni']; ?></td>
            <td><input type="submit" name="<?php echo $single_row['TID']; ?>" value="Zrušit"></td>
        </tr>
        <?php endforeach; ?>
        </table>
        <br>
        </form>
        <?php endif; ?>


        <?php if (isset($_SESSION['borrowings']) && sizeof($_SESSION['borrowings']) > 0): ?>
        <br>
        <h3 align="center">Výpůjčky</h3>
        <table align="center">
        <tr>
            <th width="28%">Název</th>
            <th width="28%">Autor</th>
            <th width="17%">Vypůjčeno dne</th>
            <th width="17%">Vráceno dne</th>
            <th width="10%">Stav</th>
        </tr>
        <?php foreach ($_SESSION['borrowings'] as $single_row): ?>
        <tr>
            <td><?php echo $single_row['Tnazev']; ?></td>
            <td><?php echo $single_row['Ajmeno'] . " " . $single_row['Aprijmeni']; ?></td>
            <td><?php echo $single_row['datum_vypujceni']; ?></td>
            <td><?php if ($single_row['datum_vraceni'] == NULL): echo "-"; else: echo $single_row['datum_vraceni']; endif; ?></td>
            <td><?php if ($single_row['stav'] == 0): echo "vráceno"; else: echo "vypůjčeno"; endif; ?></td>
        </tr>
        <?php endforeach; ?>
        </table>
        <?php endif; ?>


        <?php if (isset($_SESSION['reservations']) && sizeof($_SESSION['reservations']) == 0 &&
        isset($_SESSION['borrowings']) && sizeof($_SESSION['borrowings']) == 0): ?>
        <div align="center" role="alert">Momentálně nemáte vytvořené žádné rezervace ani výpůjčky.</div><br>
        <?php endif; ?>
    <br>
    </body>
</html> 