import numpy as np
from keras.applications.vgg16 import VGG16
from keras.applications.vgg16 import preprocess_input as preprocess_input_vgg
# from keras.applications.resnet50 import ResNet50
# from keras.applications.densenet import DenseNet121
from keras.preprocessing import image
from numpy import linalg as LA
import os
import h5py

def get_imlist(path):
    return [os.path.join(path, f) for f in os.listdir(path) if f.endswith('.jpg')]

class VGGNet:
    def __init__(self):
        self.input_shape = (224, 224, 3)
        self.weight = 'imagenet'
        self.pooling = 'max'
        # include_top：是否保留顶层的3个全连接网络
        # weights：None代表随机初始化，即不加载预训练权重。'imagenet'代表加载预训练权重
        # input_tensor：可填入Keras tensor作为模型的图像输出tensor
        # input_shape：可选，仅当include_top=False有效，应为长为3的tuple，指明输入图片的shape，图片的宽高必须大于48，如(200,200,3)
        # pooling：当include_top = False时，该参数指定了池化方式。None代表不池化，最后一个卷积层的输出为4D张量。‘avg’代表全局平均池化，‘max’代表全局最大值池化。
        # classes：可选，图片分类的类别数，仅当include_top = True并且不加载预训练权重时可用。
        self.model_vgg = VGG16(weights=self.weight,
                               input_shape=(self.input_shape[0], self.input_shape[1], self.input_shape[2]),
                               pooling=self.pooling, include_top=False)
        self.model_vgg.predict(np.zeros((1, 224, 224, 3)))

    '''
    Use vgg16/Resnet model to extract features
    Output normalized feature vector
    '''

    # 提取vgg16最后一层卷积特征
    def vgg_extract_feat(self, img_path):
        img = image.load_img(img_path, target_size=(self.input_shape[0], self.input_shape[1]))
        img = image.img_to_array(img)
        img = np.expand_dims(img, axis=0)
        img = preprocess_input_vgg(img)
        feat = self.model_vgg.predict(img)
        norm_feat = feat[0] / LA.norm(feat[0])
        return norm_feat

if __name__ == "__main__":
    database = 'database'
    index = 'models/vgg_featureCNN.h5'
    img_list = get_imlist(database)

    print("--------------------------------------------------")
    print("         feature extraction starts")
    print("--------------------------------------------------")

    feats = []
    names = []

    model = VGGNet()
    for i, img_path in enumerate(img_list):
        norm_feat = model.vgg_extract_feat(img_path)  # 修改此处改变提取特征的网络
        img_name = os.path.split(img_path)[1]
        feats.append(norm_feat)
        names.append(img_name)
        print("extracting feature from image No. %d , %d images in total" % ((i + 1), len(img_list)))

    feats = np.array(feats)

    # directory for storing extracted features
    # output = args["index"]
    output = index
    print("--------------------------------------------------")
    print("      writing feature extraction results ...")
    print("--------------------------------------------------")

    h5f = h5py.File(output, 'w')
    h5f.create_dataset('dataset_1', data=feats)
    # h5f.create_dataset('dataset_2', data = names)
    h5f.create_dataset('dataset_2', data=np.string_(names))
    h5f.close()
