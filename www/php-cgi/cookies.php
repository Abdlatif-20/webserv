<?php
	if (isset($_POST["name"]))
	{
		setcookie("name", $_POST["name"], time() + (86400 * 30), "/");
		header('Location: cookies.php');
		exit();
	}
	else if (isset($_GET["name"]))
	{
		setcookie("name", $_GET["name"], time() + (86400 * 30), "/");
		header('Location: cookies.php');
		exit();
	}
?>
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link rel="stylesheet" href="/css/style.css">
	<title>Document</title>
</head>
<body>
	<div class="diva">	
		<h2>Cookies example</h2>
		<?php
			if (isset($_COOKIE["name"]))
				echo "<h3>name is : ".$_COOKIE["name"]."</h3>";
			else
			{
		?>
		<div class="div">
			<div>
				<h3>Using GET</h3>
				<div class="div">
					<form action="/php-cgi/cookies.php" method="GET">
						<input type="text" name="name" placeholder="Name">
						<input type="submit" value="save">
					</form>
				</div>
			</div>

			<div style='margin: 50px;'> </div>

			<div>
				<h3>Using POST</h3>
				<div class="div">
					<form action="/php-cgi/cookies.php" method="POST">
						<input type="text" name="name" placeholder="Name">
						<input type="submit" value="save">
					</form>
				</div>
			</div>
		</div>
		<?php
			}
		?>
		<a href="/php-cgi"> go back </a>
	</div>
</body>
</html>