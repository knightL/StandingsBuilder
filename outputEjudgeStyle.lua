function problemName( id )
	if 1<=id and id<=26 then
		return string.char(string.byte("A")+id-1)
	else
		return problemName( math.floor((id-1)/26)+1)..string.char(string.byte("A")+(id-1)%26)
	end
end

function timeToString( time )
	if time<0 then
		return "-:--"
	else
		time=math.floor(time/60)
		local hh=math.floor(time/60)
		local mm=time%60
		return string.format("%d:%02d",hh,mm)
	end
end

function problemInfo( team, problem )
	local solved=contest.teamsolved(team,problem)
	local attempt=contest.teamproblemattempts(team,problem)
	local time=contest.teamproblemtime(team,problem)
	if not solved and attempt==0 then
		return "&nbsp;"
	elseif not solved then
		return "-"..attempt
	elseif solved and attempt==0 then
		return "+ <div>("..timeToString(time)..")</div>"
	else
		return "+"..attempt.." <div>("..timeToString(time)..")</div>"
	end
end

function cell( str)
	return "<td>"..str.."</td>"
end

function getPercent( num, den )
	if den==0 then
		return string.format("%.0f%%", 0)
	else
		return string.format("%.0f%%", num*100/den)
	end
end

function printStandings ( file, teams, time )
	
	n=#teams
	p=contest.countproblems()

	total={}
	success={}
	totalsum=0
	successsum=0

	for i=1,p do 
		total[i]=0
		success[i]=0
	end
	
	for i=1,p do
		for j,name in ipairs(teams) do
			total[i]=total[i]+contest.teamproblemattempts(name, i)
			if contest.teamsolved(name, i) then
				total[i]=total[i]+1
				success[i]=success[i]+1
			end
		end
	end

	for i=1,p do
		totalsum=totalsum+total[i]
		successsum=successsum+success[i]
	end

	file:write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/><title>Положение участников</title></head><body>\n");  
	file:write("<h1>Положение участников ["..timeToString(time).."]</h1>\n")
	file:write("<table border=\"1\">\n")
	file:write("<tr>")
	file:write("<th>Место</th><th>Участник</th>")
	
	for i=1,p do
		file:write("<th>"..problemName(i).."</th>")
	end	

	file:write("<th>Всего</th><th>Штраф</th>")
	file:write("</tr>\n")

	for i,team in ipairs(teams) do
		cstyle=contest.teamstyle(team)
		if #cstyle>0 then
			file:write("<tr bgcolor=\""..cstyle.."\">")
		else
			file:write("<tr>")
		end
		file:write(cell(contest.teamplace(team)))
		file:write(cell(team))
		for j=1,p do
			file:write(cell(problemInfo(team,j)))
		end
		file:write(cell(contest.teamproblemssolved(team)))
		file:write(cell(contest.teampenalty(team)))
		file:write("</tr>\n")
	end
	file:write("<tr>"..cell("&nbsp;")..cell("Всего:"))
	for i=1,p do
		file:write(cell(total[i]))
	end
	file:write(cell(totalsum)..cell("&nbsp").."</tr>\n")

	file:write("<tr>"..cell("&nbsp;")..cell("Успешно:"))
	for i=1,p do
		file:write(cell(success[i]))
	end
	file:write(cell(successsum)..cell("&nbsp").."</tr>\n")

	file:write("<tr>"..cell("&nbsp;")..cell("%:"))
	for i=1,p do
		file:write(cell(getPercent(success[i],total[i])))
	end
	file:write(cell(getPercent(successsum,totalsum))..cell("&nbsp").."</tr>\n")

	file:write("</table></body></html>\n");
end
