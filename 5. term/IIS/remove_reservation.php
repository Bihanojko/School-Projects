<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';

reset($_POST);
$titleID = key($_POST);

$sql = "DELETE FROM rezervace WHERE TID='".$titleID."' AND CC='".$_SESSION['UC']."'";

$result = mysqli_query($conn, $sql);

mysqli_free_result($result);
$conn->close();
header('Location: get_user_info.php');
?>