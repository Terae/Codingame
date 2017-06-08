import input_data
# Base de donnée d'images de TensorFlow
mnist = input_data.read_data_sets(raw_input(), raw_input(), raw_input())
import tensorflow as tf

# Entrée du programme
x = tf.placeholder(tf.float32, [None, 784])
# Variables de modélisation du modèle
W = tf.Variable(tf.zeros([784, 10]))
b = tf.Variable(tf.zeros([10]))
# Définition du modèle
y = tf.nn.softmax(tf.matmul(x, W) + b)

# Entraînement
y_ = tf.placeholder(tf.float32, [None, 10])
cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(y), reduction_indices=[1]))
train_step = tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)

init = tf.initialize_all_variables()

# Prédiction de 1000 images données en entrée selon le réseau de neurones entraîné
sess = tf.Session()
sess.run(init)
for i in range(1002):
	batch_xs, batch_ys = mnist.train.next_batch(100)
	sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys})

# Test de la précision du réseau (environ 92%)
result = sess.run(tf.argmax(y,1), feed_dict={x: mnist.validation.images})
print ' '.join(map(str, result))