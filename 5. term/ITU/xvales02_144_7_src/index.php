<?php header("Content-Type: text/html; charset=UTF-8");?>

<!DOCTYPE html>
<html>
	<head>
		<meta name="viewport" content="width=device-width, initial-scale=1.0" content="text/html; charset=utf-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		<link rel="stylesheet" type="text/css" href="style.css">
		<script>
			var add_to = {
				TO_READ: "to_read",
				READ: "read"
			};

			var backup = [];
			var last_detail;

			function add_to_list(name, type)
			{
				var tmp = name.parentElement.parentElement.id;
				if (tmp[0] == 't')
					tmp = tmp.substring(3);
				var id = type + "_" + tmp;
				if (document.getElementById(id) == null)
				{
					name = name.parentElement.previousElementSibling.previousElementSibling.previousElementSibling.previousElementSibling.previousElementSibling.previousElementSibling;
					var div = document.getElementById(type);
					var action = "onclick=\"remove_from_list(this)\"";
					action = "<img class=\"remove\" src=\"remove.png\"" + action + ">";
					action = name.innerHTML + action;
					document.getElementById(type).innerHTML = div.innerHTML + "<div class=\"item\" id=\"" + id + "\">" + action + "</div>";
				}
			}

			function remove_from_list(name)
			{
				name.parentNode.parentNode.removeChild(name.parentNode);
			}

			function db_operations()
			{
				var xmlHttp;
				if (window.XMLHttpRequest)
					xmlHttp = new XMLHttpRequest();
				else
					xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");

				xmlHttp.open("GET", "http://www.stud.fit.vutbr.cz/~xvales03/ITU/db_operations.php?search=" + document.getElementById("search_string").value, true);
				xmlHttp.onreadystatechange = function()
				{
					if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
					{
						document.getElementById("content_id").innerHTML = xmlHttp.responseText;
					}
				}
				xmlHttp.send(null);
				return false;
			}

			function write_rewiev(name)
			{
				name = name.parentNode;
				var div = name.parentNode;
				backup[div.id] = name.innerHTML;
				if (div.id[0] == 't')
				{
					var content = "<textarea maxlength=\"160\" class=\"large_rewiew_wr\"></textarea>";
					content = content + "<button class=\"large_button_add\" type=\"button\" onclick=\"finish_rewiev(this)\">Uložit recenzi</button>";
				}
				else
				{
					var content = "<textarea maxlength=\"160\" class=\"rewiew_wr\"></textarea>";
					content = content + "<button class=\"button_add\" type=\"button\" onclick=\"finish_rewiev(this)\">Uložit recenzi</button>";
				}
				name.innerHTML = content;
			}

			function finish_rewiev(name)
			{
				var content = name.previousElementSibling.value;
				name = name.parentNode;
				var div = name.parentNode;

				var tmp = div.id;
				if (tmp[0] == 't')
					tmp = tmp.substring(3);

				if (content != "")
				{
					var xmlHttp;
					if (window.XMLHttpRequest)
						xmlHttp = new XMLHttpRequest();
					else
						xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");

					xmlHttp.open("GET", "http://www.stud.fit.vutbr.cz/~xvales03/ITU/db_insert.php?book=" + tmp + "&rew=" + content, true);
					xmlHttp.onreadystatechange = function()
					{
						if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
						{
							name.innerHTML = backup[div.id];
						}
					}
					xmlHttp.send(null);
				}
				else
				{
					name.innerHTML = backup[div.id];
				}
			}

			function next(name)
			{
				name = name.parentNode;
				var book = name.parentNode.parentNode;
				var tmp = book.id.substring(3);
				var xmlHttp;
				if (window.XMLHttpRequest)
					xmlHttp = new XMLHttpRequest();
				else
					xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");

				xmlHttp.open("GET", "http://www.stud.fit.vutbr.cz/~xvales03/ITU/db_find_next.php?book=" + tmp + "&rew=" + name.id, true);
				xmlHttp.onreadystatechange = function()
				{
					if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
					{
						book.innerHTML = xmlHttp.responseText;
					}
				}
				xmlHttp.send(null);
			}

			function first(id)
			{
				var book = document.getElementById("top" + id);
				var xmlHttp;
				if (window.XMLHttpRequest)
					xmlHttp = new XMLHttpRequest();
				else
					xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");

				xmlHttp.open("GET", "http://www.stud.fit.vutbr.cz/~xvales03/ITU/db_find_next.php?book=" + id + "&rew=0", true);
				xmlHttp.onreadystatechange = function()
				{
					if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
					{
						book.innerHTML = xmlHttp.responseText;
					}
				}
				xmlHttp.send(null);
			}

			function prew(name)
			{
				name = name.parentNode;
				var book = name.parentNode.parentNode;
				var tmp = book.id.substring(3);
				var xmlHttp;
				if (window.XMLHttpRequest)
					xmlHttp = new XMLHttpRequest();
				else
					xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");

				xmlHttp.open("GET", "http://www.stud.fit.vutbr.cz/~xvales03/ITU/db_find_prew.php?book=" + tmp + "&rew=" + name.id, true);
				xmlHttp.onreadystatechange = function()
				{
					if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
					{
						book.innerHTML = xmlHttp.responseText;
					}
				}
				xmlHttp.send(null);
			}

			function detail(name)
			{
				content = name.parentNode.parentNode.parentNode;
				last_detail = content.id;
				content = content.innerHTML;
				var doc = document.getElementById("body");
				var gr = "<div id=\"grayout\"></div>";
				var top = "<div class=\"top\" id=\"top" + last_detail + "\"></div>";
				doc.innerHTML = gr + top + doc.innerHTML;
				first(last_detail);
			}

			function back()
			{
				var element = document.getElementById("grayout");
				element.parentNode.removeChild(element);
				element = document.getElementById("top" + last_detail);
				element.parentNode.removeChild(element);
			}
		</script>
	</head>

	<body id="body">
		<div class="search_bar">
			<div class="search_bar_pos">
				<!-- SEARCH BUTTON -->
				<form onsubmit="return db_operations()">
					<input id="search_string" type="text" size="20" style="height:28px;font-size:15pt;border:2px solid #1d242b;">&nbsp;&nbsp;
					<input type="submit" value="Hledej" style="height:33px;width:100px;font-size:15pt;background-color:#2F2F2F;color: white;font-weight: bold;">
				</form> 
				<!-- /SEARCH BUTTON -->
			</div>	
		</div>
	
		<div class="list_wiev" id="to_read">
			<p class="field_name">
				K přečtení
			</p>
		</div>

		<div class="list_wiev" id="read">
			<p class="field_name">
				Přečtené
			</p>
		</div>

		<div class="content" id="content_id">

			<?php require 'search_db.php';?>

		</div>
	</body>
</html>
