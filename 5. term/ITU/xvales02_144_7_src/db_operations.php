<?php
header("Content-Type: text/html; charset=UTF-8");
mb_internal_encoding("UTF-8");

require 'connect.php';

$author = $_GET["search"];

$sql = "SELECT * FROM Autor A, Titul T, Recenze R WHERE A.IDAutor = T.IDAutor AND T.IDTitul = R.IDTitul";

$result = mysqli_query($conn, $sql);

function check($row)
{
	global $author;
	if ($author === "")
		return true;
	if (mb_strpos(mb_strtolower($row["Jmeno"] . " " . $row["Prijmeni"]), mb_strtolower($author)) === false && mb_strpos(mb_strtolower($row["Nazev"]), mb_strtolower($author)) === false)
		return false;
	return true;
}

if ($result->num_rows > 0)
{
	$idt;
	while($row = $result->fetch_assoc())
	{
		if (!isset($idt[$row["IDTitul"]]) && check($row))
		{
			$idt[$row["IDTitul"]] = true;
			$img_url = "Covers/" . $row["IDTitul"] . ".jpg";
			echo <<< EOL
				<div class="list_book" id="{$row["IDTitul"]}">
					<p class="name">
						{$row["Nazev"]}
					</p>
					<p class="author">
						{$row["Jmeno"]} {$row["Prijmeni"]}
					</p>
					<div>
						<div class="rewiew" id="{$row["IDRecenze"]}">
							<img style="width:139px;height:210px;margin-left:20px;" src={$img_url}>
							<button type="button" class="details" onclick="detail(this)">Detail knihy</button>
						</div>
					</div>
				</div>

EOL;
		}
	}
}
$conn->close();
?>
