<?php

$servername = "localhost" ;
$Username = "u422366820_kusum_tara";
$password = "kusumTara2022" ;
$dbname = "u422366820_ece_iot" ;


$conn = new mysqli( $servername , $Username , $password , $dbname);

if ($conn->connect_error)
{
	die ('Connection failed' . $conn->connect_error);
}
?>