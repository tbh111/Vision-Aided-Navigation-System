import h5py
import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
from train import VGGNet
import sys
class VGGMatcher:
    def __init__(self):
        self.mode = 0
        self.index = 'models/vgg_featureCNN.h5'
        self.result = 'database'
        h5f = h5py.File(self.index, 'r')
        self.feats = h5f['dataset_1'][:]
        self.imgNames = h5f['dataset_2'][:]
        h5f.close()
        # init VGGNet16 model
        self.model = VGGNet()

    def match_image(self, image_name):
        print("--------------------------------------------------")
        print("               searching starts")
        print("--------------------------------------------------")

        image = mpimg.imread(image_name)
        # extract query image's feature, compute simlarity score and sort
        queryVec = self.model.vgg_extract_feat(image_name)
        scores = np.dot(queryVec, self.feats.T)

        # scores = np.dot(queryVec, feats.T)/(np.linalg.norm(queryVec)*np.linalg.norm(feats.T))
        rank_ID = np.argsort(scores)[::-1]
        rank_score = scores[rank_ID]
        # print (rank_ID)
        print(rank_score)

        # number of top retrieved images to show
        maxres = 1  # 检索出相似度最高的图片
        imlist = []
        for i, index in enumerate(rank_ID[0:maxres]):
            imlist.append(self.imgNames[index])
            # print(type(imgNames[index]))
            print("image names: " + str(self.imgNames[index]) + " scores: %f" % rank_score[i])
        print("top %d images in order are: " % maxres, imlist)
        # show top #maxres retrieved result one by one
        for i, im in enumerate(imlist):
            image = mpimg.imread(self.result + "/" + str(im, 'utf-8'))
            # plt.title("search output %d" % (i + 1))
            # plt.imshow(image)
            # plt.show()

if __name__ == "__main__":
    matcher = VGGMatcher()
    # matcher.match_image("1615126946055343.jpg")
    matcher.match_image(sys.argv[1])

