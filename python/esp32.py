
# Importing libraries.

import urllib.request
import urllib.error
import numpy
import cv2

url = "http://yourIpAddress"

# Defining the ranges for the color red in HSV format.

first_low_red = numpy.array([0,100,20],numpy.uint8)
first_high_red = numpy.array([8,255,255],numpy.uint8)

second_low_red = numpy.array([175,100,20],numpy.uint8)
second_high_red = numpy.array([179,255,255],numpy.uint8)

while True:

    try:

        # Getting a bmp image from its URL.

        with urllib.request.urlopen(f'{url}/bmp', timeout=5) as img_response:

            # Reading the bytes from the response and transforming them into an array of 8 bit unsigned integers.

            img_np = numpy.array(bytearray(img_response.read()), dtype=numpy.uint8)

            # Decoding the array into an image that preserves the original format.

            img = cv2.imdecode(img_np, -1)

            if img is not None:

                # Transforming the image to a BGR color space.

                img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

                # Creating binary masks.

                mask_red_1 = cv2.inRange(img_hsv,first_low_red,first_high_red)
                mask_red_2 = cv2.inRange(img_hsv,second_low_red,second_high_red)

                # Combining both binary masks into one.

                mask_red = cv2.add(mask_red_1,mask_red_2)

                # Getting the percentage of red pixels in the image.

                total_pixels = img.shape[0] * img.shape[1]
                red_pixels = cv2.countNonZero(mask_red)
                red_percentage = (red_pixels / total_pixels) * 100 if total_pixels > 0 else 0

                # Identifying contours in the binary mask.

                contours, _ = cv2.findContours(mask_red, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

                if contours:

                    # Getting the contour with the largest area in the binary mask.

                    largest_contour = max(contours, key=cv2.contourArea)

                    # Getting the area of the largest contour in the binary mask.

                    area = cv2.contourArea(largest_contour)

                    # Creating a bounding rectangle for the largest contour in the binary mask.

                    x, y, w, h = cv2.boundingRect(largest_contour)

                    # Calculating the aspect ratio of the bounding rectangle of the largest contour.

                    aspect_ratio = w/h

                    # Calculating the convex hull of the largest contour.

                    hull = cv2.convexHull(largest_contour)

                    # Calculating the area of the convex hull of the largest contour.

                    hull_area = cv2.contourArea(hull)

                    # Calculating the solidity of the largest contour.

                    solidity = area / hull_area if hull_area > 0 else 0

                    # Setting activation criteria (which are gradable)

                    if (area > 200 and area < 3000) and red_percentage > 0.40:

                        urllib.request.urlopen(f"{url}/control?var=led_intensity&val=1", timeout=5)

                    else:

                        urllib.request.urlopen(f"{url}/control?var=led_intensity&val=0", timeout=5)
            else:

                print("There was an error decoding the BMP image.")

    except urllib.error.URLError:

        print("There was an error accessing the URL.")

    except TimeoutError:

        print("The connection to the URL timed out.")

    except Exception as e:

        print(f"Something unexpected went wrong: {e}")