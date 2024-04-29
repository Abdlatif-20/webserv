<?php
	session_save_path("/tmp");
	session_start();
	if (isset($_POST["name"]))
	{
		$_SESSION["name"] = $_POST["name"];
		header('Location: session.php');
		exit();
	}
	else if (isset($_GET["name"]))
	{
		$_SESSION["name"] = $_GET["name"];
		header('Location: session.php');
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
		<h2>Session example</h2>
		<?php
			if (isset($_SESSION["name"]))
				echo "<h3>name is : ".$_SESSION["name"]."</h3>";
			else
			{
		?>
		<div class="div">
			<div>
				<h3>Using GET</h3>
				<div class="div">
					<form action="/php-cgi/session.php" method="GET">
						<input type="text" name="name" placeholder="Name">
						<input type="submit" value="save">
					</form>
				</div>
			</div>

			<div style='margin: 50px;'> </div>

			<div>
				<h3>Using POST</h3>
				<div class="div">
					<form action="/php-cgi/session.php" method="POST">
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