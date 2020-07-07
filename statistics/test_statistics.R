#Visualisation of dot, food and tick count. 

#install ggplot2:
install.packages("ggplot2")

library(ggplot2)

#read of file:
dot_data = read.csv("/home/saloberg/Schreibtisch/Freizeit/Evolution/dot_count.csv", header=TRUE, sep = ";")
food_data = read.csv("/home/saloberg/Schreibtisch/Freizeit/Evolution/food_count.csv", header=TRUE, sep = ";")


#preparing data 
#selection
start = 400
end = 600 #upto 5000

dot_data = dot_data[start:end,]
food_data = food_data[start:end,]
#concatination
dot_food_data <- cbind.data.frame(dot_data, food_data$food_count)


#adding a group for the color in ggplot
dot_data["group"] = rep(2,each = nrow(dot_data))
food_data["group"] = rep(4, each = nrow(food_data))


#visualisation: 
ggplot(dot_food_data, aes(x = ticknumber))+
  geom_line(aes(y = dot_data$dot_count), color = dot_data$group)+
  geom_line(aes(y = food_data$food_count), color = food_data$group)+
  scale_x_continuous(name="Tick") +
  scale_y_continuous(name="Count") + 
  ggtitle("Dots (red) vs Food (blue)")
    
