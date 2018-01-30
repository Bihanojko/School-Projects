<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';

$title = $_GET['TID'];

unset($_SESSION['title_info']);

$sql = "SELECT * FROM titul T, autor A, titul_autor TA WHERE TA.TID=T.TID AND A.AC=TA.AC AND T.TID='".$title."'";
$result = mysqli_query($conn, $sql);

if ($result->num_rows > 0)
    $_SESSION['title_info'] = $result->fetch_assoc();

mysqli_free_result($result);
$conn->close();
header('Location: title_info.php');
?>