SELECT
	"wnd_name",
	TOTAL("dp_end" - "dp_start") AS "wnd_seconds"
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
