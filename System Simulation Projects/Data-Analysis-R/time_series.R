library("xlsx")
std_error <- function(x) sd(x)/sqrt(length(x))
pexp_interval <- function(a,b,rate) pexp(b, rate) - pexp(a, rate)


data <- read.xlsx("input.xls", sheetIndex = 1)

day1 <- data[['Day.1']]
day2 <- data[['Day.2']]
time_series_1 = vector(,length(day1))
time_series_1[1] = day1[1]

index <- 2
for (i in seq(2, length(day1), 1)){
  time_series_1[index] <- day1[i] + time_series_1[i-1]
  index <- index+1
}

day1_timeseries <- hist(time_series_1,xlim=c(0,20000), breaks=40)


time_series_2 = vector(,length(day2))
time_series_2[1] = day2[1]

index <- 2
for (i in seq(2, length(day2), 1)){
  time_series_2[index] <- day2[i] + time_series_2[i-1]
  index <- index+1
}

day2_timeseries <- hist(time_series_2, xlim=c(0,20000), breaks=40)
