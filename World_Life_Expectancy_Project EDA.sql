-- World Life Expectancy Project ( Exploratory Data Analysis)
-- Analysiing Life expectancy how it has increased and decreased
-- Correlation


SELECT *
FROM world_life_expectancy;

SELECT Country, MIN(`Life expectancy`), MAX(`Life expectancy`),
ROUND(MAX(`Life expectancy`) - MIN(`Life expectancy`),1) AS Life_Increase_Over_15_Years
FROM world_life_expectancy
GROUP BY Country
HAVING  MIN(`Life expectancy`) != 0
AND  MIN(`Life expectancy`) != 0
ORDER BY  Life_Increase_Over_15_Years DESC;

SELECT  Country ,ROUND(AVG(`Life expectancy`),1) AS Life_Exp,ROUND(AVG(GDP),1) AS GDP
FROM world_life_expectancy
GROUP BY Country
HAVING Life_Exp > 0
AND GDP > 0
ORDER BY GDP ASC;

SELECT 
SUM(CASE
		WHEN GDP >= 1500 THEN 1 
        ELSE 0
END) High_GDP,
AVG(CASE
		WHEN GDP >= 1500 THEN `Life expectancy`
        ELSE null
END) High_GDP_Life_expectancy,
SUM(CASE
		WHEN GDP <= 1500 THEN 1 
        ELSE 0
END) Low_GDP,
AVG(CASE
		WHEN GDP <= 1500 THEN `Life expectancy`
        ELSE null
END) Low_GDP_Life_expectancy
FROM world_life_expectancy;


SELECT Status, ROUND(AVG(`Life expectancy`),1)
FROM world_life_expectancy
GROUP BY Status;

SELECT Status, COUNT(DISTINCT Country), ROUND(AVG(`Life expectancy`),1)
FROM world_life_expectancy
GROUP BY Status;

SELECT  Country ,ROUND(AVG(`Life expectancy`),1) AS Life_Exp,ROUND(AVG(BMI),1) AS BMI
FROM world_life_expectancy
GROUP BY Country
HAVING Life_Exp > 0
AND BMI > 0
ORDER BY BMI DESC;

SELECT  Country, Year, `Life expectancy`, `Adult Mortality`,
SUM(`Adult Mortality`) OVER(PARTITION BY COUNTRY ORDER BY Year) AS Rolling_total
FROM world_life_expectancy
WHERE Country LIKE '%UNITED%';