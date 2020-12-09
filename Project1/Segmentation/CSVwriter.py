from PIL import Image
from os import listdir

class CSVwriter:

    def __init__(self, ImagesFolderPath, SourceTextPath=None, ImgSize=None):
        self.ImagesFolderPath = ImagesFolderPath #chemin du dossier qui contenant les images
        self.SourceTextPath = SourceTextPath #chemin du fichier contenant le text "source"
        self.imageNameList = self.imageNameList()
        if ImgSize:
            self.ImgSize = ImgSize
            self.resize = True
        else:
            image = Image.open("{}/{}".format(ImagesFolderPath, self.imageNameList[0]))
            self.ImgSize = image.size
            self.resize = False
        self.imagePxValueList = self.imagePxValueList()
        self.textCharList = self.textCharList()

    def write(self, filePath):
        charLength = len(self.textCharList)
        if len(self.imageNameList) != charLength:
            print("error : not the same number of image as letters")
            return None
        file = open(filePath, "w")
        file.write("{},{}\n".format(self.ImgSize[0], self.ImgSize[1]))
        for i in range(charLength):
            file.write("{},{}\n".format(self.textCharList[i], "{}".format(self.imagePxValueList[i])[1:-1].replace(' ', '') ))
            #print("{}/{}".format(i+1, charLength))

    def showImg(self, imgIndex):
        img = Image.new("RGB", self.ImgSize, (255, 255, 255))
        i = 0
        for y in range(self.ImgSize[1]):
            for x in range(self.ImgSize[0]):
                val = self.imagePxValueList[imgIndex][i]
                img.putpixel((x, y),(val, val, val))
                i += 1
        img.show()

    def saveImg(self, imgIndex):
        img = Image.new("RGB", self.ImgSize, (255, 255, 255))
        i = 0
        for y in range(self.ImgSize[1]):
            for x in range(self.ImgSize[0]):
                val = self.imagePxValueList[imgIndex][i]
                img.putpixel((x, y),(val, val, val))
                i += 1
        img.save("imgSave.png", "PNG")

    def textCharList(self):
        excluedChar = ["\n", " "]
        file = open(self.SourceTextPath,'r')
        result = []
        for line in file:
            for char in line:
                if char not in excluedChar:
                    result.append(char)
        return result

    def imagePxValueList(self):
        result = []
        for imageName in self.imageNameList:
            image = Image.open("{}/{}".format(self.ImagesFolderPath, imageName))
            if self.resize:
                image = image.resize(self.ImgSize)
            pixels = image.load()
            list = []
            for y in range(self.ImgSize[1]):
                for x in range(self.ImgSize[0]):
                    try:
                        list.append(pixels[x, y][0])
                    except:
                        print("error : images are not the same size")
                        return [["Error"]]*len(self.imageNameList)
            result.append(list)
        return result

    def imageNameList(self):
        result = []
        for i in range(len(listdir(self.ImagesFolderPath))):
            result.append("{}.png".format(i+1))
        return result


a = CSVwriter("image", "text.txt", (28, 28))
a.write("fichier.csv")
