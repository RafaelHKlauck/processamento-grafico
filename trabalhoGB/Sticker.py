import cv2

class Sticker:
  def __init__(self, posX, posY, scale, image):
    self.posX = posX
    self.posY = posY
    self.scale = scale

    width = int(image.shape[1] * scale / 100)
    height = int(image.shape[0] * scale / 100)
    dim = (width, height)
    self.image = cv2.resize(image, dim, interpolation = cv2.INTER_AREA)