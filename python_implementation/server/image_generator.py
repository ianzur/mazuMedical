"""image_generator.py

So I don't have to stare at my ugly mug all the time

And is helpful to see if the ordering is as expected

TODO: Add arg to return as float, currently always returns uint8 
"""

import cv2
import numpy as np

class imgGenerator:
    """generate sequences of numbered images"""

    def __init__(self, img_width: int=640, img_height: int=480, gray: bool=False):

        self.count = 0
        
        self.img_width = img_width
        self.img_height = img_height
        self.gray = gray

        self.font = cv2.FONT_HERSHEY_SIMPLEX
        self.thickness = 3
        self.size = self._get_optimize_font_size()
        
    
    def _get_optimize_font_size(self):
        
        # arbitrarily chosen, but I want at least this number to fit on the image
        n = 9999
        size = 0

        # reversed so exit on the biggest text that would fit
        for size in reversed(range(1, 20)):

            size_x, _ = cv2.getTextSize(str(n), self.font, size, self.thickness)[0]

            if (size_x > (self.img_width * .666)) and (size_x < (self.img_width * .75)):
                break

        return size


    def __next__(self):
        
        shape = (self.img_height, self.img_width, 1 if self.gray else 3)
        img = np.ones(shape=shape)

        # center text on image
        textsize = cv2.getTextSize(str(self.count), self.font, self.size, self.thickness)[0]
        text_x = (self.img_width - textsize[0]) // 2
        text_y = (self.img_height + textsize[1]) // 2

        cv2.putText(
            img, 
            str(self.count),
            (text_x, text_y),
            self.font, 
            self.size, 
            (0,0,0),
            thickness=self.thickness
        )

        self.count += 1
        return (img * 255).astype(np.uint8)


if __name__ == "__main__":

    ### example usage ###
    img_gen = imgGenerator(img_width=1080, img_height=720)

    while True:

        img = next(img_gen)

        cv2.imshow("example", img)

        key = cv2.waitKey(500)

        # "q" or escape to quit
        if key == ord("q") or key == 27:
            break

    cv2.destroyAllWindows()