# 	US Household Income Exploratory Data Analysis

SELECT  State_Name, SUM(ALand), SUM(AWater)
FROM us_household_project.us_household_income
GROUP BY State_Name
ORDER BY 2 DESC
LIMIT 10;

SELECT * 
FROM us_household_project.ushouseholdincome_statistics;


SELECT * 
FROM us_household_project.us_household_income ui
JOIN us_household_project.ushouseholdincome_statistics us
ON ui.id = us.id;

SELECT * 
FROM us_household_project.ushouseholdincome_statistics;



-- SELECT * 
-- FROM us_household_project.us_household_income ui
-- RIGHT JOIN us_household_project.ushouseholdincome_statistics us
-- ON ui.id = us.id
-- WHERE ui.id is NULL;

SELECT ui.State_Name, ROUND(AVG(Mean),1), ROUND(AVG(Median),1)
FROM us_household_project.us_household_income ui
INNER JOIN us_household_project.ushouseholdincome_statistics us
ON ui.id = us.id
WHERE Mean <> 0
GROUP BY ui.State_Name
ORDER BY 2 DESC
LIMIT 10;


SELECT Type,COUNT(Type), ROUND(AVG(Mean),1), ROUND(AVG(Median),1)
FROM us_household_project.us_household_income ui
INNER JOIN us_household_project.ushouseholdincome_statistics us
ON ui.id = us.id
WHERE Mean <> 0
GROUP BY Type
HAVING COUNT(Type) > 100
ORDER BY 2 DESC
LIMIT 20;


SELECT ui.State_Name, City, round(AVG(MEAN),1)
FROM us_household_project.us_household_income ui
JOIN us_household_project.ushouseholdincome_statistics us
ON ui.id = us.id
GROUP BY ui.State_Name, City
ORDER BY 3 Desc ;
