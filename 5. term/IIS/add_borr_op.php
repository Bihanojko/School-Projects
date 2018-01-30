<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';
$_SESSION['error'] = 0;

$prirustkove_cislo = $CC = "";

$prirustkove_cislo = $_POST["prirustkove_cislo"];
$CC = $_POST['CC'];
$next_page = "user_data.php?UC=";

$sql = "SELECT * FROM vypujcka WHERE prirustkove_cislo='". $prirustkove_cislo . "' AND stav=1";

$result = mysqli_query($conn, $sql);

if ($result->num_rows > 0)
{
    $_SESSION['error'] = 5;
    mysqli_free_result($result);
    $conn->close();
    $next_page = "add_borrowing.php?UC=";
}

$sql = "INSERT INTO vypujcka (datum_vypujceni, stav, prirustkove_cislo, CC, OC_vytvoril)
VALUES (now(),1,".$prirustkove_cislo.",".$CC.",".$_SESSION['UC'].")";

if ($conn->query($sql) === TRUE)
{
    $sql = "SELECT * FROM vypujcka V, vytisk VY, titul T, uzivatel U, autor A, titul_autor TA WHERE V.prirustkove_cislo='".$prirustkove_cislo."' AND V.stav=1 AND V.prirustkove_cislo=VY.prirustkove_cislo AND VY.TID=T.TID AND V.CC=U.UC AND TA.TID=T.TID AND A.AC=TA.AC";

    $result = mysqli_query($conn, $sql);

    if ($result->num_rows > 0)
    {
        $row = $result->fetch_assoc();
        for ($i = 0; $i < count($_SESSION['users']); $i++)
        {
            if ($_SESSION['users'][$i]['UC'] == $CC)
                $_SESSION['users'][$i]['borrowings'][] = $row;
        }
    }
    $_SESSION['error'] = 4;
} 

else
{
    $_SESSION['error'] = 5;
    echo "Error: " . $sql . "<br>" . $conn->error;
}

mysqli_free_result($result);
$conn->close();
header('Location: ' . $next_page . $CC);
?>