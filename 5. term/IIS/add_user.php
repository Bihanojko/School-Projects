<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';
$_SESSION['error'] = 0;

$name = $surname = $login = $passwd = $email = $street = $city = $psc = $phone = $user_type = "";

$name = $_POST["name"];
$surname = $_POST["surname"];
$login = $_POST["login"];
$passwd = crypt($_POST["passwd"]);
$email = $_POST["email"];
$street = $_POST["street"];
$city = $_POST["city"];
$psc = $_POST["psc"];
$phone = $_POST["phone"];
$user_type = $_POST["user_type"];

$sql = "INSERT INTO uzivatel (jmeno, prijmeni, login, heslo, typ_uziv, ulice, mesto, psc, email, tel)
VALUES ('".$name."','".$surname."','".$login."','".$passwd."','".$user_type."','".$street."','".$city."','".$psc."','".$email."','".$phone."')";

if ($conn->query($sql) === TRUE)
{
    if ($_POST['next_page'] != "add_user_form.php")
    {
        $_SESSION['login'] = $login;
        $_SESSION['usertype'] = '0';
        $_SESSION['register'] = '1';
    }
} 
else
{
    $_SESSION['error'] = 1;
    if ($_POST['next_page'] != "add_user_form.php")
        $_POST['next_page'] = "registration.php";
}

$sql = "SELECT * FROM uzivatel WHERE login='".$login."'";

$result = mysqli_query($conn, $sql);

if ($result->num_rows > 0)
{
    $row = $result->fetch_assoc();
    $_SESSION['UC'] = $row['UC'];
}

mysqli_free_result($result);
$conn->close();
header('Location: ' . $_POST['next_page']);
?>