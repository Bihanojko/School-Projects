<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';

unset($_SESSION['borrowings']);
unset($_SESSION['reservations']);
$_SESSION['borrowings'] = array();
$_SESSION['reservations'] = array();

$sql = "SELECT * FROM vypujcka V, vytisk VY, titul T, uzivatel U, autor A, titul_autor TA WHERE V.prirustkove_cislo=VY.prirustkove_cislo AND VY.TID=T.TID AND V.CC=U.UC AND TA.TID=T.TID AND A.AC=TA.AC AND U.login='".$_SESSION['login']."'";
$result = mysqli_query($conn, $sql);

if ($result->num_rows > 0)
{
    while ($row = $result->fetch_assoc())
        array_push($_SESSION['borrowings'], $row);
}

$sql = "SELECT * FROM rezervace R, titul T, uzivatel U, autor A, titul_autor TA WHERE R.TID=T.TID AND R.CC=U.UC AND TA.TID=T.TID AND A.AC=TA.AC AND U.login='".$_SESSION['login']."'";
$result = mysqli_query($conn, $sql);

if ($result->num_rows > 0)
{
    while ($row = $result->fetch_assoc())
        array_push($_SESSION['reservations'], $row);
}

mysqli_free_result($result);
$conn->close();
header('Location: user_info.php');
?>