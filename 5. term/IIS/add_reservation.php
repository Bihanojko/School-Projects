<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';

reset($_POST);
$titleID = key($_POST);

$sql = "INSERT INTO rezervace (TID, CC) VALUES ('".$titleID."','".$_SESSION['UC']."')";

$result = mysqli_query($conn, $sql);

for ($i = 0; $i < count($_SESSION['rows']); $i++)
{
    if ($_SESSION['rows'][$i]['TID'] == $titleID)
        $_SESSION['rows'][$i]['is_reserved'] = 1;
}

mysqli_free_result($result);
$conn->close();
header('Location: intraportal.php');
?>