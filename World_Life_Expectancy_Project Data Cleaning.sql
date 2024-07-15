-- World Life Expectancy project (Data Cleaning)
-- Trying to identify duplicates and then remove them
-- Trying to identify blank values in the status

SELECT *
 FROM world_life_expectancy;
 
 
 
 
 SELECT Country , Year, concat(Country, Year), count(concat(Country, Year))
 FROM world_life_expectancy
 GROUP BY  Country , Year
 HAVING  count(concat(Country, Year)) > 1 ;
 
 
 SELECT *
 FROM (
			SELECT ROW_ID,
			CONCAT(Country, Year),
			ROW_NUMBER() OVER( PARTITION BY CONCAT(Country, Year) ORDER BY Country, Year) as ROW_NUM
			FROM world_life_expectancy
            ) AS Row_table
WHERE ROW_NUM >1
;

DELETE FROM world_life_expectancy
WHERE
			ROW_ID IN (
            SELECT ROW_ID
 FROM (
			SELECT ROW_ID,
			CONCAT(Country, Year),
			ROW_NUMBER() OVER( PARTITION BY CONCAT(Country, Year) ORDER BY Country, Year) as ROW_NUM
			FROM world_life_expectancy
            ) AS Row_table
WHERE ROW_NUM >1);

SELECT *
FROM world_life_expectancy
WHERE Status = '';





SELECT DISTINCT(Status)
 FROM world_life_expectancy
 WHERE Status <> ' '
;

SELECT DISTINCT( Country)
 FROM world_life_expectancy
 WHERE Status = 'Developing';
 
--  UPDATE world_life_expectancy
--  SET Status = 'Developing'
--  WHERE Country in ( SELECT DISTINCT( Country)
-- 								FROM world_life_expectancy
-- 								WHERE Status = 'Developing');
 UPDATE world_life_expectancy t1
 JOIN world_life_expectancy t2
		on t1.Country = t2.Country
SET t1.Status = 'Developing'
WHERE t1.Status =  ' '
AND t2.Status <> ' '
AND T2.Status = 'Developing';
 
SELECT *
FROM world_life_expectancy
WHERE Country = 'United States of America';

  UPDATE world_life_expectancy t1
 JOIN world_life_expectancy t2
		on t1.Country = t2.Country
SET t1.Status = 'Developed'
WHERE t1.Status =  ''
AND t2.Status != ''
AND T2.Status = 'Developed';

SELECT *
FROM world_life_expectancy;
-- WHERE `Life expectancy` = '';

SELECT t1.Country , t1.Year, t1.`Life expectancy`, t2.Country , t2.Year, t2.`Life expectancy`,
t3.Country , t3.Year, t3.`Life expectancy`,
ROUND((t2. `Life expectancy` +  t3. `Life expectancy`) /2,1)
FROM world_life_expectancy t1
JOIN world_life_expectancy t2
			on t1.Country = t2.Country
            AND t1.Year = t2.Year -1
JOIN world_life_expectancy t3
			on t1.Country = t3.Country
            AND t1.Year = t3.Year +1
WHERE t1.`Life expectancy` = '';

UPDATE world_life_expectancy t1
JOIN world_life_expectancy t2
			on t1.Country = t2.Country
            AND t1.Year = t2.Year -1
JOIN world_life_expectancy t3
			on t1.Country = t3.Country
            AND t1.Year = t3.Year +1
SET  t1.`Life expectancy` = ROUND((t2. `Life expectancy` +  t3. `Life expectancy`) /2,1)
WHERE  t1.`Life expectancy` = '';
            


