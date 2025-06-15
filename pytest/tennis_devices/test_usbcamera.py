import cv2

if __name__ == "__main__":
    # Open the default camera (usually the first camera)
    cap = cv2.VideoCapture(2)   # check 'ls /dev/video*' -> PC: 2/3; Pi: 0

    if not cap.isOpened():
        print("Error: Could not open camera.")
        exit()

    frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS)

    print(f"Default frame width: {frame_width}")
    print(f"Default frame height: {frame_height}")
    print(f"Default FPS: {fps}")

    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()

        if not ret:
            print("Error: Could not read frame.")
            break

        # Display the resulting frame
        cv2.imshow('Camera Feed', frame)

        # Break the loop on 'q' key press
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the camera and close all OpenCV windows
    cap.release()
    cv2.destroyAllWindows()