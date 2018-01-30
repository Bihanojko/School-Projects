<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';
$_SESSION['error'] = 0;

$title = $aname = $asurname = $language = $genre = $type = $isbnissn = "";

$title = $_POST["title"];
$aname = $_POST["aname"];
$asurname = $_POST["asurname"];
$language = $_POST["language"];
$genre = $_POST["genre"];
$type = $_POST["type"];
$isbnissn = $_POST["isbnissn"];

$sql = "SELECT * FROM autor WHERE Ajmeno='".$aname."' AND Aprijmeni='".$asurname."'";
$result = mysqli_query($conn, $sql);

if ($result->num_rows > 0)      // autor uz existuje
{
    $row = $result->fetch_assoc();
    $AC = $row['AC'];
}
else        // autora pridat
{
    $sql = "INSERT INTO autor (Ajmeno, Aprijmeni) VALUES ('".$aname."','".$asurname."')";
    if ($conn->query($sql) !== TRUE)    // TODO error
    {    
        $_SESSION['error'] = 1;
    }

    $sql = "SELECT * FROM autor WHERE Ajmeno='".$aname."' AND Aprijmeni='".$asurname."'";

    $result = mysqli_query($conn, $sql);

    if ($result->num_rows > 0)
    {
        $row = $result->fetch_assoc();
        $AC = $row['AC'];
    }
}

if ($type == 'kniha')
    $sql = "INSERT INTO titul (Tnazev, jazyk, zanr, typ, ISBN) VALUES ('".$title."','".$language."','".$genre."','1','".$isbnissn."')";
else
    $sql = "INSERT INTO titul (Tnazev, jazyk, zanr, typ, ISSN) VALUES ('".$title."','".$language."','".$genre."','2','".$isbnissn."')";

if ($conn->query($sql) !== TRUE)    // TODO error
{
    $_SESSION['error'] = 1;
}

$sql = "SELECT * FROM titul WHERE Tnazev='".$title."'";
$result = mysqli_query($conn, $sql);

if ($result->num_rows > 0)
{
    $row = $result->fetch_assoc();
    $TID = $row['TID'];
}

$sql = "INSERT INTO titul_autor (TID, AC) VALUES ('".$TID."','".$AC."')";

if ($conn->query($sql) !== TRUE)    // TODO error
{
    $_SESSION['error'] = 1;
}

mysqli_free_result($result);
$conn->close();
header('Location: add_title.php');
?>