<?php
session_start();
header("Content-Type: text/html; charset=UTF-8");
require 'connect.php';

if (isset($_POST['save'])) // save changes
{
    $_SESSION['error'] = 0;
    
    $sql = "UPDATE autor SET Ajmeno='".$_POST['aname']."', Aprijmeni='".$_POST['asurname']."' WHERE AC='".$_POST['AC']."'";
    if (isset($_POST['isbn']))
    {
        $sql2 = "UPDATE titul SET Tnazev='".$_POST['title']."', jazyk='".$_POST['language']."', zanr='".$_POST['genre']."', ISBN='".$_POST['isbn']."'  WHERE TID='".$_POST['titulID']."'";
        $_SESSION['title_info']['ISBN'] = $_POST['isbn'];  
    }
    else
    {
        $sql2 = "UPDATE titul SET Tnazev='".$_POST['title']."', jazyk='".$_POST['language']."', zanr='".$_POST['genre']."', ISSN='".$_POST['issn']."'  WHERE TID='".$_POST['titulID']."'";
        $_SESSION['title_info']['ISSN'] = $_POST['issn'];          
    }

    $_SESSION['title_info']['Ajmeno'] = $_POST['aname'];    
    $_SESSION['title_info']['Aprijmeni'] = $_POST['asurname'];  
    $_SESSION['title_info']['Tnazev'] = $_POST['title'];  
    $_SESSION['title_info']['jazyk'] = $_POST['language'];  
    $_SESSION['title_info']['zanr'] = $_POST['genre'];  
   
    for ($i = 0; $i < count($_SESSION['rows']); $i++)
    {
        if ($_SESSION['rows'][$i]['TID'] == $_POST['titulID'])
        {
            $_SESSION['rows'][$i]['Tnazev'] = $_POST['title'];
            $_SESSION['rows'][$i]['zanr'] = $_POST['genre'];
        }
        if ($_SESSION['rows'][$i]['AC'] == $_POST['AC'])
        {
            $_SESSION['rows'][$i]['Ajmeno'] = $_POST['aname'];
            $_SESSION['rows'][$i]['Aprijmeni'] = $_POST['asurname'];
        }
    }

    if ($conn->query($sql) === FALSE || $conn->query($sql2) === FALSE)
        $_SESSION['error'] = 1;

    $next_page = "title_info.php";
}

else // remove record
{
    $next_page = "intraportal.php";

    $sql = "SELECT T.prirustkove_cislo FROM vytisk T, vypujcka P WHERE T.prirustkove_cislo=P.prirustkove_cislo AND T.TID='".$_POST['titulID']."'";
    $result = mysqli_query($conn, $sql);

    if ($result->num_rows > 0)
    {
        while ($row = $result->fetch_assoc())
        {
            $sql = "DELETE FROM vypujcka WHERE prirustkove_cislo='".$row['prirustkove_cislo']."'";
            if ($conn->query($sql) === FALSE)
            {
                $next_page = "title_info.php";
                $_SESSION['error'] = 2;
            }
        }
    }
    mysqli_free_result($result);

    $sql1 = "DELETE FROM titul_autor WHERE TID='".$_POST['titulID']."'";
    $sql2 = "DELETE FROM rezervace WHERE TID='".$_POST['titulID']."'";
    $sql3 = "DELETE FROM vytisk WHERE TID='".$_POST['titulID']."'";
    $sql4 = "DELETE FROM titul WHERE TID='".$_POST['titulID']."'";

    if ($conn->query($sql1) === FALSE || $conn->query($sql2) === FALSE ||
    $conn->query($sql3) === FALSE || $conn->query($sql4) === FALSE)
    {
        $next_page = "title_info.php";
        $_SESSION['error'] = 2;
    }

    else
    {
        for ($i = 0; $i < count($_SESSION['rows']); $i++)
        {
            if ($_SESSION['rows'][$i]['TID'] == $_POST['titulID'])
            {
                unset($_SESSION['rows'][$i]);
                break;
            }
        }
    }

}

$conn->close();
header('Location: ' . $next_page);
?>