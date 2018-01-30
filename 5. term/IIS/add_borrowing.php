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
                var txt = document.getElementById("jmeno").value;
                if (txt.match(/^[0-9]+$/i))
                {
                    document.getElementById("jmeno").style.color = "black";
                    document.getElementById("submit").disabled = false;
                }
                else
                {
                    document.getElementById("submit").disabled = true; 
                    if (txt.length == 0)                    
                        document.getElementById("jmeno").style.color = "black";   
                    else                
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
        <h2 align="center">Vytvoření výpůjčky</h2><br>

        <?php if (isset($_SESSION['error']) && $_SESSION['error'] == 5): ?>
            <div align="center" role="alert"><strong>Chyba při vytváření výpůjčky, výtisk je momentálně vypůjčen.</strong></div><br>
        <?php unset($_SESSION['error']); endif; ?>

        <?php foreach ($_SESSION['users'] as $single_row):
            if (isset($_GET['UC']) && $single_row['UC'] == $UC):
                $row = $single_row;
            endif; endforeach; ?>

        <br>
        <form action="add_borr_op.php" method="POST"> 
        <table align="center" width="28%">
        <tr>
            <th width="50%">Čtenář</th>
            <td width="50%"><?php echo $row['jmeno'] . " " . $row['prijmeni']; ?></td>
        </tr>
        <tr>
            <th>Login</th>
            <td><?php echo $row['login']; ?></td>
         </tr>
        <tr>
            <th>Přírůstkové číslo výtisku (5 - 21)</th>
            <td><input type="text" maxlength="50" name="prirustkove_cislo" id="jmeno" oninput="check_add_title()"></td>
        </tr>
        </table>
        <br><input type="submit" value="Vytvořit výpůjčku" id="submit" disabled>
        <input type="hidden" value="<?php echo $UC;?>" name="CC">
        </form>
        <br>

    </body>
</html>