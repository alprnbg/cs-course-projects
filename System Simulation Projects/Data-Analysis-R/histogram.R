file_path = ''
data <- read_xls(file_path)


hist(data$`Day 1`,breaks=seq(0,488,5), xlab="Inter-arrival Times(sec)", main="Distribution of Day1, Bin Size = 5")
hist(data$`Day 1`,breaks=seq(0,488,10), xlab="Inter-arrival Times(sec)", main="Distribution of Day1, Bin Size = 10")
hist(data$`Day 1`,breaks=seq(0,488,20), xlab="Inter-arrival Times(sec)", main="Distribution of Day1, Bin Size = 20")


hist(data$`Day 2`,breaks=seq(0,488,5), xlab="Inter-arrival Times(sec)", main="Distribution of Day2, Bin Size = 5")
hist(data$`Day 2`,breaks=seq(0,488,10), xlab="Inter-arrival Times(sec)", main="Distribution of Day2, Bin Size = 10")
hist(data$`Day 2`,breaks=seq(0,488,20), xlab="Inter-arrival Times(sec)", main="Distribution of Day2, Bin Size = 20")

