from ultralytics import YOLO
import cv2

# Load the trained YOLOv8 model
model = YOLO("Waste_Segreagtion_Project.pt")  # Replace with path to your .pt file if needed

# Open the webcam (0 = default camera)
cap = cv2.VideoCapture(0)

# Check if the webcam is opened
if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()

print("Running YOLOv8... Press 'q' to quit.")

while True:
    ret, frame = cap.read()
    if not ret:
        print("Failed to grab frame.")
        break

    # Run YOLOv8 on the frame
    results = model(frame, verbose=False)

    # Draw results on the frame
    annotated_frame = results[0].plot()

    # Display the frame
    cv2.imshow("YOLOv8 Webcam Detection", annotated_frame)

    # Break on 'q' key
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release resources
cap.release()
cv2.destroyAllWindows()
