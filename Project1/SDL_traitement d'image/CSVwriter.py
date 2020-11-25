import csv
from PIL import Image
from os import listdir

class CSVwriter:
    def __init__(self, ImagesFolderPath, SourceTextPath=None, ImgSize=None):
        self.ImagesFolderPath = ImagesFolderPath #chemin du dossier qui contenant les images
        self.SourceTextPath = SourceTextPath #chemin du fichier contenant le text "source"
        if ImgSize:
            self.ImgSize = ImgSize
        else:
            image = Image.open("{}/{}".format(ImagesFolderPath, self.imageNameList[0]))
            self.ImgSize = image.size

    def write(self, filePath):
        charLength = len(self.textCharList)
        if len(self.imageNameList) != charLength:
            print("error : not the same number of image as letters")
            return None
        file = open(filePath, "w", newline="")
        theWriter = csv.writer(file)
        theWriter.writerow(["{}*{}".format(self.ImgSize[0], self.ImgSize[1])])
        for i in range(charLength):
            theWriter.writerow([self.textCharList[i], self.imageNameList[i], self.imagePxValueList[i]])

    @property
    def textCharList(self):
        excluedChar = ["\n", " "]
        file = open(self.SourceTextPath,'r')
        result = []
        for line in file:
            for char in line:
                if char not in excluedChar:
                    result.append(char)
        return result

    @property
    def imagePxValueList(self):
        result = []
        for imageName in self.imageNameList:
            image = Image.open("{}/{}".format(self.ImagesFolderPath, imageName))
            pixels = image.load()
            list = []
            for x in range(self.ImgSize[0]):
                for y in range(self.ImgSize[1]):
                    try:
                        if pixels[x, y][0] > 127:
                            list.append(0)
                        else:
                            list.append(1)
                    except:
                        print("error : images are not the same size")
                        return [["Error"]]*len(self.imageNameList)
            result.append(list)
        return result

    @property
    def imageNameList(self):
        return listdir(self.ImagesFolderPath)

a = CSVwriter("image", "test.txt")
a.write("fichier.csv")
