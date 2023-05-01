<?php

$servername = "localhost";

// REPLACE with your Database name
$dbname = "u422366820_ece_iot";
// REPLACE with Database user
$username = "u422366820_kusum_tara";
// REPLACE with Database user password
$password = "kusumTara2022";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "1234";

$api_key=  $value1 = "";



if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $json = file_get_contents('php://input');
    $data = json_decode($json);
    $api_key = test_input($data->api_key);
    if($api_key == $api_key_value) {
        for($i = 0 ; $i < 1500 ; $i++){
        $value1 = test_input( $data->value1[$i]);

        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO SensorData (value1)
        VALUES ('" . $value1 . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}