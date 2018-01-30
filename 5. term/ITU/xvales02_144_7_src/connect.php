<?php
$servername = "localhost";
$username = "xvales02";
$password = "4edokame";
$dbname = "xvales02";


// Create connection
$conn = new mysqli($servername, $username, $password);
mysqli_set_charset($conn, "UTF8");

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$conn->select_db($dbname);
?> 
