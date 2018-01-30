<?php
header("Content-Type: text/html; charset=UTF-8");

require 'connect.php';

$book_id = $_GET["book"];
$rew_id = $_GET["rew"];
$first = null;
$count = 0;
$position = 0;

$sql = "SELECT * FROM Autor A, Titul T, Recenze R WHERE A.IDAutor = T.IDAutor AND T.IDTitul = R.IDTitul AND T.IDTitul = " . $book_id;

$result = mysqli_query($conn, $sql);

if ($result->num_rows > 0)
{
	while($result->fetch_assoc())
	{
		$count++;
	}
}

$sql = "SELECT * FROM Autor A, Titul T, Recenze R WHERE A.IDAutor = T.IDAutor AND T.IDTitul = R.IDTitul";

$result = mysqli_query($conn, $sql);

function check($row)
{
	global $book_id;
	global $rew_id;
	global $first;
	global $position;

	if ($row["IDTitul"] == $book_id)
		$position++;

	if ($first === null && $row["IDTitul"] == $book_id)
		$first = $row;

	if ($row["IDTitul"] == $book_id && $row["IDRecenze"] > $rew_id)
		return true;
	return false;
}

if ($result->num_rows > 0)
{
	$match = false;
	while($row = $result->fetch_assoc())
	{
		if (check($row))
		{	
			$img_url = "Covers/" . $row["IDTitul"] . ".jpg";
			echo <<< EOL
				<img class="back" src="remove.png"onclick="back()">
				<p class="large_name">
					&ensp;{$row["Nazev"]}
				</p>
				<p class="large_author">
					{$row["Jmeno"]} {$row["Prijmeni"]}
				</p>
				<div style="display: inline-block;vertical-align:top;">
						<img style="width:200px;height:300px;margin-left:30px;margin-right:-20px;margin-bottom:34px;" src={$img_url}>
					<div class="large_rewiew" id="{$row["IDRecenze"]}">
						<img src="sipka24.png" style="width:70px;height:35px;margin-left:40px;margin-top:10px;" onclick="prew(this)">
						<img src="sipka14.png" style="width:70px;height:35px;float:right;margin-right:40px;margin-top:10px;" onclick="next(this)"><br>
						<center><p class="review_content">Obsah recenze {$position}/{$count}</p></center>
						<p style="margin:15px;">{$row["Obsah"]}</p>
					</div>
					<button class="large_button_add" type="button" onclick="write_rewiev(this)">Napsat recenzi</button>
				</div>
				<br><br><br>
				<p class="list_buttons">
					<button class="large_button" type="button" onclick="add_to_list(this, add_to.TO_READ)">Přidat do seznamu K přečtení</button>
					<button class="large_button" type="button" onclick="add_to_list(this, add_to.READ)">Přidat do seznamu Přečtené</button>
				</p>
EOL;
			$match = true;
			break;
		}
	}

	if (!$match)
	{
		global $first;
		$row = $first;
		$img_url = "Covers/" . $row["IDTitul"] . ".jpg";
		echo <<< EOL
			<img class="back" src="remove.png"onclick="back()">
			<p class="large_name">
				&ensp;{$row["Nazev"]}
			</p>
			<p class="large_author">
				{$row["Jmeno"]} {$row["Prijmeni"]}
			</p>
			<div style="display: inline-block;vertical-align:top;">
					<img style="width:200px;height:300px;margin-left:30px;margin-right:-20px;margin-bottom:34px;" src={$img_url}>
				<div class="large_rewiew" id="{$row["IDRecenze"]}">
					<img src="sipka24.png" style="width:70px;height:35px;margin-left:40px;margin-top:10px;" onclick="prew(this)">
					<img src="sipka14.png" style="width:70px;height:35px;float:right;margin-right:40px;margin-top:10px;" onclick="next(this)"><br>
					<center><p class="review_content">Obsah recenze {$position}/{$count}</p></center>
					<p style="margin:15px;">{$row["Obsah"]}</p>
				</div>
				<button class="large_button_add" type="button" onclick="write_rewiev(this)">Napsat recenzi</button>
			</div>
			<br><br><br>
			<p class="list_buttons">
				<button class="large_button" type="button" onclick="add_to_list(this, add_to.TO_READ)">Přidat do seznamu K přečtení</button>
				<button class="large_button" type="button" onclick="add_to_list(this, add_to.READ)">Přidat do seznamu Přečtené</button>
			</p>
EOL;
	}
}
$conn->close();
?>
