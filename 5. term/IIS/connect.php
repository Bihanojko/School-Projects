<?php
$servername = "localhost";
$username = "xvales03";
$password = "mojfu2no";
$dbname = "xvales03";
header("Content-Type: text/html; charset=UTF-8");

// Create connection
$conn = new mysqli($servername, $username, $password);
mysqli_set_charset($conn, "UTF8");

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$conn->select_db($dbname);
?> 