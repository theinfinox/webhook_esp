<?php
// Check if the request method is POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Get the JSON data from the request body
    $json_data = file_get_contents('php://input');
    
    // Decode the JSON data
    $data = json_decode($json_data, true);
    
    // Check if JSON decoding was successful
    if ($data !== null) {
        // Extract FingerID from the data
        $fingerID = isset($data['FingerID']) ? $data['FingerID'] : '';
        
        // Validate FingerID (You can add more validation if required)
        if (!empty($fingerID)) {
            // Connect to MySQL database
            $servername = "localhost";
            $username = "root";		//put your phpmyadmin username.(default is "root")
            $password = "";		
            $dbname = "test_db";

            $conn = new mysqli($servername, $username, $password, $dbname);

            // Check connection
            if ($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
            }

            // Prepare SQL statement
            $sql = "INSERT INTO test (FingerID) VALUES (?)";

            // Prepare and bind parameters
            $stmt = $conn->prepare($sql);
            $stmt->bind_param("s", $fingerID);

            // Execute the statement
            if ($stmt->execute()) {
                echo "Data inserted successfully!";
            } else {
                echo "Error: " . $sql . "<br>" . $conn->error;
            }

            // Close statement and connection
            $stmt->close();
            $conn->close();
        } else {
            echo "Error: FingerID is empty or not provided!";
        }
    } else {
        echo "Error: Invalid JSON data!";
    }
} else {
    echo "Error: Only POST requests are allowed!";
}
?>
