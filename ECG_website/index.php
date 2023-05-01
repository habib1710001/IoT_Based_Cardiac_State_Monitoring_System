<?php
session_start();
?>

<!DOCTYPE html>
<html>
<head>
	<title>index</title>
</head>
<body> 
       <?php 
       if(isset($_SESSION["useruid"])){
       	echo "<a href='home.php'>Home</a>";
       	echo "<a href='includes/logout.inc.php'>log Out</a>";
       }
       else{
       	echo"<a href='login.php'>login</a>
            <a href='signup.php'>signup</a>";
       }
        ?>

</body>
</html>