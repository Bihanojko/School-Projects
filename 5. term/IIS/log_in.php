<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';

$userID = $_POST["userid"];
$passwd = $_POST["passwd"];

$sql = "SELECT UC, login, heslo, typ_uziv FROM uzivatel WHERE login='".$userID."'"; 

$result = mysqli_query($conn, $sql);

if ($result->num_rows == 1)
{
    $row = $result->fetch_assoc();
    $_SESSION['login'] = $row["login"];

    if ($row["heslo"] == crypt($passwd, $row['heslo']))       // successful
    {
        $_SESSION['UC'] = $row["UC"];
        $_SESSION['usertype'] = $row["typ_uziv"];

         $next_page = "intraportal.php";
    }
    else    // wrong password
    {
        $_SESSION['error'] = "1";
        $next_page = "index.php";
    }
}
else        // invalid username
{   
    $_SESSION['error'] = "2";
    mysqli_free_result($result);
    $conn->close();
    $next_page = "index.php";
}

mysqli_free_result($result);
$conn->close();
header('Location: ' . $next_page);
?>