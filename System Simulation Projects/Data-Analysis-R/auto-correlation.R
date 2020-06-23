file_path = ''
data <- read_xls(file_path)

# Auto correlation function
acf(data$`Day 1`, lag.max = 2, plot = TRUE)
acf(data$`Day 1`, lag.max = 2, plot = FALSE)
acf(data$`Day 2`, lag.max = 2, plot = TRUE)
acf(data$`Day 2`, lag.max = 2, plot = FALSE)