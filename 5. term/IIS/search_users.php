<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';

$search = $_POST["search"];
unset($_SESSION['users']);
$_SESSION['users'] = array();

$sql = "SELECT * FROM uzivatel";

$result = mysqli_query($conn, $sql);

if ($result->num_rows > 0)
{
    while ($row = $result->fetch_assoc())
    {
        if (((strpos(strtolower($row['jmeno']. " " . $row['prijmeni']), strtolower($search)) !== FALSE) ||
        (strpos(strtolower($row['login']), strtolower($search)) !== FALSE)) && $row['typ_uziv'] <= $_SESSION['usertype'])
            array_push($_SESSION['users'], $row);
    }
}

for ($i = 0; $i < count($_SESSION['users']); $i++)
{
    $_SESSION['users'][$i]['borrowings'] = array();

    $sql = "SELECT * FROM vypujcka V, vytisk VY, titul T, uzivatel U, autor A, titul_autor TA WHERE V.prirustkove_cislo=VY.prirustkove_cislo AND VY.TID=T.TID AND V.CC=U.UC AND TA.TID=T.TID AND A.AC=TA.AC AND U.login='".$_SESSION['users'][$i]['login']."'";
    $result = mysqli_query($conn, $sql);

    if ($result->num_rows > 0)
    {
        while ($row = $result->fetch_assoc())
            $_SESSION['users'][$i]['borrowings'][] = $row;
    }   
}

mysqli_free_result($result);
$conn->close();
header('Location: users_db.php');
?>