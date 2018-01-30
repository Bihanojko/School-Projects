<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';

reset($_POST);
$VID = key($_POST);

$sql = "UPDATE vypujcka SET stav=0, OC_uzavrel='".$_SESSION['UC']."', datum_vraceni=now() WHERE VID='".$VID."'";

if ($conn->query($sql) !== FALSE)
{
    $conn->close();

    for ($i = 0; $i < count($_SESSION['users']); $i++)
    {
        for ($j = 0; $j < count($_SESSION['users'][$i]['borrowings']); $j++)
        {
            if ($_SESSION['users'][$i]['borrowings'][$j]['VID'] == $VID)
            {
                $_SESSION['users'][$i]['borrowings'][$j]['stav'] = 0;
                $_SESSION['users'][$i]['borrowings'][$j]['datum_vraceni'] = date("Y-m-d");
                header('Location: user_data.php?UC=' . $_SESSION['users'][$i]['UC']);
            }
        }
    }
}
?>