<?php
header("Content-Type: text/html; charset=UTF-8");

require 'connect.php';

$titleID = $_GET["book"];
$content = $_GET["rew"];

echo $titleID;
echo $content;

$sql = "INSERT INTO Recenze (IDTitul, Obsah) VALUES ('".$titleID."','".$content."')";

$result = mysqli_query($conn, $sql);

$conn->close();
?>
