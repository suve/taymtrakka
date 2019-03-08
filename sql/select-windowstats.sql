--
-- This file is part of Taym Trakka.
-- Copyright (C) 2019 Artur "suve" Iwicki
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License, version 3,
-- as published by the Free Software Foundation.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License along with
-- this program (LICENCE.txt). If not, see <http://www.gnu.org/licenses/>.
--
SELECT
	"wnd_name",
	TOTAL(
		CASE WHEN "dp_end" <= ?2 THEN "dp_end" ELSE ?2 END
		- 
		CASE WHEN "dp_start" >= ?1 THEN "dp_start" ELSE ?1 END
	) AS "wnd_seconds"
FROM
	"datapoints"
LEFT JOIN
	"windows"
ON
	"datapoints"."dp_wnd_id" = "windows"."wnd_id"
WHERE
	("dp_end" >= ?1)
AND
	("dp_start" <= ?2)
GROUP BY
	"dp_wnd_id"
ORDER BY
	"wnd_seconds" DESC
;
