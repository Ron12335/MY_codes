USE walmart_db;
select * from walmart;

select count(*) from walmart;

select payment_method, count(*) from walmart
group by payment_method;

select count(distinct Branch) from walmart;

select max(quantity) from walmart;

select min(quantity) from walmart;

-- Questions
-- What are the different payment methods, and how many transactions and items were sold with each method?

select payment_method, count(*),sum(quantity) as qnt from walmart
group by payment_method;

-- Which category received the highest average rating in each branch?
select *
from
(select Branch, category , avg(rating) as avg_rating, 
rank() over(partition by Branch order by avg(rating) desc ) as rank_1
from walmart
group by Branch,category) as ranked_data
where rank_1 = 1;

-- What is the busiest day of the week for each branch based on transaction volume?
SELECT 
    Branch,
    DAYNAME(STR_TO_DATE(date, '%d/%m/%y')) AS day_name,
    count(*) as num_transactions
FROM 
    walmart
group by Branch, day_name;

-- How many items were sold through each payment method?
select payment_method,count(*), sum(quantity) as qnt from walmart
group by payment_method;

-- What are the average, minimum, and maximum ratings for each category in each city?
select category, City, round(avg(rating),2) as avg_rating, min(rating) as min_rating, max(rating) as max_rating
from walmart
group by category, city;

-- What is the total profit for each category, ranked from highest to lowest?

SELECT 
    category,
    SUM(unit_price * quantity * profit_margin) AS total_profit
FROM walmart
GROUP BY category
ORDER BY total_profit DESC;

-- What is the most frequently used payment method in each branch?
select *
from
(select Branch, payment_method, count(*) as total_transactions,
rank() over(partition by Branch order by count(*) desc) as rank_1
from walmart
group by Branch, payment_method) as pay
where rank_1 = 1;

-- Which branches experienced the largest decrease in revenue compared to the previous year?

SELECT 
    r2022.branch,
    r2022.revenue AS last_year_revenue,
    r2023.revenue AS current_year_revenue,
    ROUND(((r2022.revenue - r2023.revenue) / r2022.revenue) * 100, 2) AS revenue_decrease_ratio
FROM 
    (SELECT 
         branch,
         SUM(total) AS revenue
     FROM walmart
     WHERE YEAR(STR_TO_DATE(date, '%d/%m/%Y')) = 2022
     GROUP BY branch) AS r2022
JOIN 
    (SELECT 
         branch,
         SUM(total) AS revenue
     FROM walmart
     WHERE YEAR(STR_TO_DATE(date, '%d/%m/%Y')) = 2023
     GROUP BY branch) AS r2023
ON 
    r2022.branch = r2023.branch
WHERE 
    r2022.revenue > r2023.revenue
ORDER BY 
    revenue_decrease_ratio DESC
LIMIT 5;
