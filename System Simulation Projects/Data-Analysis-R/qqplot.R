file_path = ''
data <- read_xls(file_path)


qqplot(x=qexp(ppoints(488)),data$`Day 1`, main="Exponential Q-Q Plot - Day1",
       xlab="Theoretical Quantiles", ylab= "Data Quantiles")
#results linear => comes from exponential dist

qqplot(x=qexp(ppoints(488)),data$`Day 2`, main="Exponential Q-Q Plot - Day2",
       xlab="Theoretical Quantiles", ylab= "Data Quantiles")
#results linear => comes from exponential dist