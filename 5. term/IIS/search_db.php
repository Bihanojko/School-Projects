<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';

$search = $_POST["search"];
$attribute = $_POST["attribute"];
unset($_SESSION['rows']);
$_SESSION['rows'] = array();

$sql = "SELECT * FROM autor A, titul T, titul_autor TA WHERE A.AC=TA.AC AND TA.TID=T.TID";
$sql2 = "SELECT * FROM rezervace WHERE CC='".$_SESSION["UC"]."'";

$result = mysqli_query($conn, $sql);
$result2 = mysqli_query($conn, $sql2);

if ($result->num_rows > 0)
{
    while ($row = $result->fetch_assoc())
    {
        if (($attribute == 'nazev' && (strpos(strtolower($row['Tnazev']), strtolower($search)) !== false)) || 
        ($attribute == 'autor' && (strpos(strtolower($row['Ajmeno']. " " . $row['Aprijmeni']), strtolower($search)) !== false)) ||
        ($attribute == 'zanr' && (strpos(strtolower($row['zanr']), strtolower($search)) !== false)) ||
        ($attribute == 'ISBN' && (strpos(strtolower($row['ISBN'], $search)) !== false)))
        {
            if ($result2->num_rows > 0)
            {
                $is_reserved = 0;
                while ($row2 = $result2->fetch_assoc())
                {
                    if ($row['TID'] == $row2['TID'])
                        $is_reserved = 1;
                }
            }
            $row['is_reserved'] = $is_reserved;
            array_push($_SESSION['rows'], $row);
            $result2->data_seek(0);
        }
    }
}

mysqli_free_result($result);
$conn->close();
header('Location: intraportal.php');
?>