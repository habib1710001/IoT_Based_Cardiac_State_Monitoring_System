<?php
session_start();

if (!$_SESSION["useruid"]) {
    header("location: http://kusumtara.com/login.php");
    exit();
}
?>
<!DOCTYPE html>
<html>
    <head>
        <title>ECG IoT</title>
        <style>
            body  {
                background-image: url("sign_cover.png");
                background-repeat: no-repeat; 
                background-size: cover;
            }
                
            li {
                width : 60%;
                padding-top :3%;
                padding-bottom :3%;
                background-color:red;
                margin-top :3%;
                margin-bottom : 3%;
                font-size:30px;
                list-style : none;
                border: 2px Red;
                border-radius: 15px;
                }
            h1{
              text-decoration:none;
              text-align:center; 
              margin-left: 10%; 
              border-radius: 0px; 
              margin:0px;  
              background-color:Red; 
              border: 2px red;
              border-radius: 5px; 
              margin-left:20%; 
              margin-right:20%; 
              color:White;
              
            }
            a{
             text-decoration:none;
             color :White;
            }
            ul{
               margin-top :5%;
               margin-bottom : 5%;
               margin-left:30%;
               margin-right:10%;
            }
        </style>
    </head>
    <body style = "text-align:center">
        <h1>IoT Based Cardiac State Monitoring System</h1>
        <ul>
            <li><a href = "table.php">ADC Data Table</a></li>
            <li><a href = "table_voltage.php">Voltage Date Table</a></li>
            <li><a href = "plot.php">Voltage Data Plot</a></li>
            <li><a href = "includes/logout.inc.php">Logout</a></li>
        </ul>
    </body>
</html>