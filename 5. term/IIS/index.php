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
        </style>

    </head>
    <body>

        <h1 align="center">Informační systém knihovny</h1>
        <h2 align="center">Přihlášení</h2><br>

        <?php if (isset($_SESSION['error']) && $_SESSION['error'] == "1"): ?>
        <div align="center" role="alert"><strong>Neplatné heslo!</strong></div><br>
        <?php unset($_SESSION['error']); endif; ?>
        <?php if (isset($_SESSION['error']) && $_SESSION['error'] == "2"): ?>
        <div align="center" role="alert"><strong>Zadáno neexistující uživatelské jméno!</strong></div><br>
        <?php unset($_SESSION['error']); endif; ?>
        <?php if (isset($_SESSION['error']) && $_SESSION['error'] == "3"): ?>
        <div align="center" role="alert"><strong>Proběhlo automatické odhlášení z důvodu delší neaktivity.</strong></div><br>
        <?php unset($_SESSION['error']); endif; ?>

        <form action="log_in.php" method="POST">
        Uživatelské jméno:<br>
        <input type="text" maxlength="50" name="userid" <?php if (isset($_SESSION['login'])): ?> value="<?php echo $_SESSION['login']; ?>" <?php endif; ?>>
        <br>
        Heslo:<br>
        <input type="password" maxlength="50" name="passwd" value="">
        <br><br>
        <input type="submit" value="Přihlásit">
        </form>
        <br>
        <p align="center"><a href="registration.php">Nemáte účet? Zaregistrujte se!</a></p>
    <br>
    </body>
</html> 