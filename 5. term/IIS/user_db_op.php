<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';

if (isset($_POST['save'])) // save changes
{
    $_SESSION['error'] = 0;             // TODO check if all * fields inserted and check values -->

    $sql = "UPDATE uzivatel SET jmeno='".$_POST['name']."', prijmeni='".$_POST['surname']."', login='".$_POST['login']."', 
    typ_uziv='".$_POST['user_type']."', ulice='".$_POST['street']."', mesto='".$_POST['city']."', psc='".$_POST['psc']."', email='".$_POST['email']."', 
    tel='".$_POST['phone']."' WHERE UC='".$_POST['UC']."'";

    for ($i = 0; $i < count($_SESSION['users']); $i++)
    {
        if ($_SESSION['users'][$i]['UC'] == $_POST['UC'])
        {
            $_SESSION['users'][$i]['name'] = $_POST['name'];
            $_SESSION['users'][$i]['surname'] = $_POST['surname'];
            $_SESSION['users'][$i]['login'] = $_POST['login'];
            $_SESSION['users'][$i]['street'] = $_POST['street'];
            $_SESSION['users'][$i]['city'] = $_POST['city'];
            $_SESSION['users'][$i]['psc'] = $_POST['psc'];
            $_SESSION['users'][$i]['email'] = $_POST['email'];
            $_SESSION['users'][$i]['phone'] = $_POST['phone'];
            $_SESSION['users'][$i]['user_type'] = $_POST['user_type'];
        }
    }

    if ($conn->query($sql) === FALSE)
        $_SESSION['error'] = 1;

    $next_page = "user_data.php?UC=" . $_POST['UC'];
}

else // remove record
{
    $next_page = "users_db.php";

    $sql1 = "DELETE FROM vypujcka WHERE CC='".$_POST['UC']."'";
    $sql2 = "DELETE FROM rezervace WHERE CC='".$_POST['UC']."'";
    $sql3 = "DELETE FROM uzivatel WHERE UC='".$_POST['UC']."'";

    if ($conn->query($sql1) === FALSE || $conn->query($sql2) === FALSE || $conn->query($sql3) === FALSE)
    {
        $next_page = "user_data.php";
        $_SESSION['error'] = 2;
    }
    else
    {
        for ($i = 0; $i < count($_SESSION['users']); $i++)
        {
            if ($_SESSION['users'][$i]['UC'] == $_POST['UC'])
            {
                unset($_SESSION['users'][$i]);
                break;
            }
        }
    }
}

$conn->close();
header('Location: ' . $next_page);
?>