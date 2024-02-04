# Results

## Checking the distribution of the data

Before performing any hypothesis tests, it's important to assess the distribution of the data. Let's start by examining the distribution of the winning percentages for both the non-blind and blind modes.

Starting from a graphical analysis, we can create histograms for both the non-blind and blind mode winning percentages to visually inspect their distributions.

![Histograms](plots/win_rates.png)
![Histograms](plots/differences.png)

The histograms show that the winning percentages for both the non-blind and blind modes are approximately normally distributed. This is further supported by the Shapiro-Wilk test for normality, which yields p-values of approximately 0.073 and 0.091 for the non-blind and blind mode winning percentages, respectively. These p-values are above the common threshold of 0.05 used to denote statistical significance, suggesting that the winning percentages for both modes do not significantly deviate from a normal distribution.
Based on these results, it's reasonable to conclude that the assumption of normality for the winning percentages in both modes is met, justifying the use of parametric tests like the paired samples t-test for analyzing these data.

## Comparing the winning percentages

The Shapiro-Wilk test for normality on the differences between the non-blind and blind mode winning percentages yields a statistic of approximately 0.984 and a p-value of approximately 0.914. This suggests that the differences between paired observations do not significantly deviate from a normal distribution, as the p-value is well above the common threshold of 0.05 used to denote statistical significance.

Additionally, the Q-Q plot visually supports this finding. The data points closely follow the 45-degree line, indicating that the differences are approximately normally distributed.

Based on these results, it's reasonable to conclude that the assumption of normality for the differences between the paired observations is met, justifying the use of parametric tests like the paired samples t-test for analyzing these data.

![Q-Q Plot](plots/qq_plot.png)

## Hypothesis test

Now that we've confirmed the normality of the data, we can proceed with the hypothesis test. We'll use a paired samples t-test to compare the winning percentages between the non-blind and blind modes.

The paired samples t-test between the non-blind and blind mode winning percentages resulted in a t-statistic of approximately **-3.67e-17** and a p-value of **1.0**.

The t-statistic is very close to zero, and the p-value is 1, indicating that there is no statistically significant difference between the winning percentages of players in non-blind and blind modes. This suggests that, on average, the mode of the game (non-blind vs. blind) does not significantly affect the winning percentages of the players in this sample.

## Conclusion

Based on the results of the hypothesis test, we fail to reject the null hypothesis. There is no statistically significant difference between the winning percentages of players in non-blind and blind modes. This suggests that, on average, the mode of the game (non-blind vs. blind) does not significantly affect the winning percentages of the players in this sample.

This finding has important implications for game design and player experience. It suggests that the blind mode does not significantly disadvantage players in terms of winning percentages compared to the non-blind mode. This may help inform decisions about game design and accessibility, as it indicates that the blind mode does not significantly impact player performance in this context.

