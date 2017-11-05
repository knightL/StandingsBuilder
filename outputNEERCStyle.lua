stylesheet="http://neerc.ifmo.ru/standings-wt.css"
title="ACM ICPC 2017-2018, NEERC"
contest_length=5*60*60

function problemName( id )
	if 1<=id and id<=26 then
		return string.char(string.byte("A")+id-1)
	else
		return problemName( math.floor((id-1)/26)+1)..string.char(string.byte("A")+(id-1)%26)
	end
end

function timeToString( time, show_hh )
	if time<0 and show_hh then
		return "--:--"
	elseif time<0 and not show_hh then
		return "-:--:--"
	elseif show_hh then
		local ss=time%60
		time=math.floor(time/60)
		local mm=time%60
		local hh=math.floor(time/60)
		return string.format("%d:%02d:%02d",hh,mm,ss)
	else
		local ss=time%60
		local mm=math.floor(time/60)
		return string.format("%d:%02d",mm,ss)
	end
end

function problemInfo( team, problem , firstac)
	local solved=contest.teamsolved(team,problem)
	local attempt=contest.teamproblemattempts(team,problem)
	local time=contest.teamproblemtime(team,problem)
	if not solved and attempt==0 then
		return "."
	elseif not solved then
		return "<b>-"..attempt.."</b>"
	elseif solved and attempt==0 and firstac then
		return "<i class=\"first-to-solve\">+<s><br>"..timeToString(time, false).."</s></i>"
	elseif solved and attempt==0 then
        return "<i>+<s><br>"..timeToString(time, false).."</s></i>"
	elseif firstac then
		return "<i class=\"first-to-solve\">+"..attempt.."<s><br>"..timeToString(time, false).."</s></i>"
	else
		return "<i>+"..attempt.."<s><br>"..timeToString(time, false).."</s></i>"
	end
end

function cell(class, str)
    if class == nil then
        return "<td>"..str.."</td>"
    else
        return "<td class=\""..class.."\">"..str.."</td>"
    end
end

function getPercent( num, den )
	if den==0 then
		return string.format("%.0f%%", 0)
	else
		return string.format("%.0f%%", num*100/den)
	end
end

function printStandings ( file, teams, time )

	if time<0 then
		time=contest_length
	end
	time=math.min(time, contest_length)
	
	n=#teams
	p=contest.countproblems()

	total={}
	success={}
    rejected={}
    firstac={}
    lastac={}

	for i=1,p do 
		total[i]=0
		success[i]=0
        rejected[i]=0
        firstac[i]=-1
        lastac[i]=-1
	end
	
	for i=1,p do
		for j,name in ipairs(teams) do
			total[i]=total[i]+contest.teamproblemattempts(name, i)
            rejected[i]=rejected[i]+contest.teamproblemattempts(name,i)
			if contest.teamsolved(name, i) then
				total[i]=total[i]+1
				success[i]=success[i]+1
				if firstac[i] == -1 or firstac[i] > contest.teamproblemtime(name,i) then
					firstac[i] = contest.teamproblemtime(name,i)
				end
				if lastac[i] == -1 or lastac[i] < contest.teamproblemtime(name,i) then
					lastac[i] = contest.teamproblemtime(name,i)
				end
			end
		end
	end

	file:write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/><title>Standings</title><script type=\"text/javascript\">\n")
    file:write("function onShowTime() {\n")
    file:write("    var showTime = document.getElementById(\"show-time\");\n")
    file:write("    var display = showTime.checked ? \"\" : \"none\";\n")
    file:write("    var times = document.getElementsByTagName('s');\n")
    file:write("    for (var i = 0; i < times.length; i++){\n")
    file:write("        times[i].style.display = display;\n")
    file:write("    }\n")
    file:write("}\n")
    file:write("</script><link rel=\"stylesheet\" href=\"".. stylesheet .."\"></head><body>\n"); 
    file:write("<table class=\"wrapper\">\n")
    file:write("<tr><td><center>\n")
    
    file:write("<a name=\""..title.."\"><h2>"..title.."</h2></a>\n")
    file:write("<p>"..timeToString(time, true).." of "..timeToString(contest_length, true).."</p>\n")
    file:write("<p><input id=\"show-time\" type=\"checkbox\" name=\"show-times\" onclick=\"onShowTime()\" checked=\"true\">Show time</p>\n")
    
    file:write("<table class=\"standings\">\n")
    file:write("<thead><tr><th class=\"rankl\">Rank</th><th class=\"party\">Team</th>")
    for i=1,p do
        file:write("<th class=\"problem\">"..problemName(i).."</th>")
    end
    file:write("<th class=\"solved\">=</th><th class=\"penalty\">Time</th></thead>\n")
    file:write("<tbody>\n")
    
    prevsolved=p+1
    pstyle=1
    parity=0
    
    for i,team in ipairs(teams) do
        if contest.teamproblemssolved(team) ~= prevsolved then
            pstyle = 1 - pstyle
            prevsolved=contest.teamproblemssolved(team)
        end
        parity = 1-parity
        cstyle=contest.teamstyle(team)
        file:write("<tr class=\"row"..pstyle..parity)
        if #cstyle>0 then
            file:write(" "..cstyle)
        end
        file:write("\">")
        file:write(cell("rankl", string.gmatch(contest.teamplace(team),"[0-9]+")()))
        file:write(cell("party", team))
        for j=1,p do
			if contest.teamsolved(team,j) and contest.teamproblemtime(team,j) == firstac[j] then
				file:write(cell(nil,problemInfo(team, j, true)))
			else
				file:write(cell(nil,problemInfo(team, j, false)))
			end
        end
        file:write(cell(nil, contest.teamproblemssolved(team)))
        file:write(cell("penalty", contest.teampenalty(team)))
        file:write("</tr>\n")
    end
    file:write("</tbody>\n")
	
	file:write("<tfoot>\n")
    file:write("<tr>"..cell(nil, "")..cell(nil,"Total runs"))
	for i=1,p do
		file:write(cell(nil, total[i]))
	end
	file:write("<tr>\n")
	
	file:write("<tr>"..cell(nil, "")..cell(nil,"Accepted"))
	for i=1,p do
		file:write(cell(nil, success[i]))
	end
	file:write("<tr>\n")
	
	file:write("<tr>"..cell(nil, "")..cell(nil,"Rejected"))
	for i=1,p do
		file:write(cell(nil, rejected[i]))
	end
	file:write("<tr>\n")
	
	file:write("<tr>"..cell(nil, "")..cell(nil,"First Accept"))
	for i=1,p do
		if firstac[i] ~= -1 then
			file:write(cell(nil, timeToString(firstac[i])))
		else
			file:write(cell(nil, ""))
		end
	end
	file:write("<tr>\n")
	
	file:write("<tr>"..cell(nil, "")..cell(nil,"Last Accept"))
	for i=1,p do
		if lastac[i] ~= -1 then
			file:write(cell(nil, timeToString(lastac[i])))
		else
			file:write(cell(nil, ""))
		end
	end
	file:write("<tr>\n")
	
    file:write("</tfoot></table>\n")
    
    file:write("</center></tr></td>\n")
    file:write("</table></body></html>\n");

end
