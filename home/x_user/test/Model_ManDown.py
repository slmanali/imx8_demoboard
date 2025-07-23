import skl2onnx
from skl2onnx import convert_sklearn
from skl2onnx.common.data_types import FloatTensorType
import matplotlib.pyplot as plt
import numpy as np
from sklearn.ensemble import RandomForestClassifier
import joblib
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, confusion_matrix

print('start')

RANDOM_SEED = 0
NUMTREES = 200

x_train_file = open('Class_features.txt', 'r')
y_train_file = open('Class_labels.txt', 'r')

# Create empty lists
x_train = []
y_train = []

labels = {'Work': 1, 'Relax': 2, 'Fall': 3}
# Loop through datasets
for x in x_train_file:
    x_train.append([float(ts) for ts in x.split()])

for y in y_train_file:
    y_train.append(int(y.rstrip('\n')))

x_train = np.array(x_train)
y_train = np.array(y_train)
print(x_train.shape)  # should print (n_samples, 17)
X_train, X_test, y_train, y_test = train_test_split(
    x_train, y_train, test_size=0.3)

rf = RandomForestClassifier(criterion='gini', n_estimators=NUMTREES)
rf.fit(X_train, y_train)
y_pred = rf.predict(X_test)
conf_mat = confusion_matrix(y_test, y_pred)
print(classification_report(y_test, y_pred))
joblib.dump(rf, "Class_Freq_R.pkl")
initial_type = [('float_input', FloatTensorType([None, 18]))]  # 17 if that's your feature size
onnx_model = convert_sklearn(rf, initial_types=initial_type)
with open("Class_Freq_R.onnx", "wb") as f:
    f.write(onnx_model.SerializeToString())
print('Finish')
# import onnx
# model = onnx.load("Class_Freq_R.onnx")
# print("Inputs:")
# for i in model.graph.input:
#     print(i.name, i.type.tensor_type.shape)
# print("Outputs:")
# for o in model.graph.output:
#     print(o.name, o.type.tensor_type.shape)
